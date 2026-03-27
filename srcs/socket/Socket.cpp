#include "Socket.hpp"


Socket::Socket(const char *port): _port(port), _sockfd(-1){}

Socket::~Socket(void)
{
	freeaddrinfo(_servinfo);
	close(_sockfd);
}

void Socket::getAddrInfo()
{
	int				status;
	struct addrinfo	hints;

	std::memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((status = getaddrinfo(NULL, _port, &hints, &_servinfo)) != 0){
		std::ostringstream oss;
		oss << "Error: getaddrinfo failed (code " << status << ")";
		throw std::runtime_error(oss.str());
	}
}

void Socket::setNonBlocking(int fd) const {
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0){
		throw std::runtime_error("Error: fcntl F_SETFL O_NONBLOCK failed");
	}
}

void	Socket::setupSocket(){
	getAddrInfo();
	_sockfd = socket(_servinfo->ai_family, _servinfo->ai_socktype, _servinfo->ai_protocol);
	if (_sockfd < 0){
		throw std::runtime_error("Error: Can't create socket");
	}
	int opt = 1;
	if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0){
		throw std::runtime_error("Error: Can't config socket");
	}
	setNonBlocking(_sockfd);
}

void	Socket::bindScoket() const{
	if (bind(_sockfd, _servinfo->ai_addr, _servinfo->ai_addrlen) < 0){
		throw std::runtime_error("Error: Can't bind socket");
	}
}

void	Socket::startListen() const{
	if (listen(_sockfd, MAX_CONNECTIONS) < 0){
		throw std::runtime_error("Error: Server can't listening");
	}
}

ClientConnection Socket::acceptClient() const{
	struct sockaddr_storage	clientAddr;
	socklen_t				addr_size = sizeof(clientAddr);

	int fd = accept(_sockfd, (struct sockaddr *)&clientAddr, &addr_size);
	if (fd < 0) return ClientConnection(fd);

	setNonBlocking(fd);

	char ip[INET6_ADDRSTRLEN];
	uint16_t port = 0;
	if (clientAddr.ss_family == AF_INET) {
		sockaddr_in* s = (sockaddr_in*)&clientAddr;
		inet_ntop(AF_INET, &s->sin_addr, ip, sizeof(ip));
		port = ntohs(s->sin_port);
	} else {
		sockaddr_in6* s = (sockaddr_in6*)&clientAddr;
		inet_ntop(AF_INET6, &s->sin6_addr, ip, sizeof(ip));
		port = ntohs(s->sin6_port);
	}

	return ClientConnection(fd, ip, port);
}

int		Socket::getSockFd(void) const{
	return _sockfd;
}

const char*	Socket::getPort() const{
	return _port;
}

void Socket::start(void)
{
	setupSocket();
	bindScoket();
	startListen();
}
