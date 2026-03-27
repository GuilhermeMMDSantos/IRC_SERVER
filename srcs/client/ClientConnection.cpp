#include "ClientConnection.hpp"

ClientConnection::ClientConnection(int fd, std::string ip, uint16_t port): _fd(fd), _ip(ip), _port(port){}

ClientConnection::ClientConnection(const ClientConnection& src): _fd(src._fd), _ip(src._ip), _port(src._port){}

int			ClientConnection::getfd() const{
	return _fd;
}

std::string	ClientConnection::getIP() const{
	return _ip;
}

uint16_t	ClientConnection::getPort() const{
	return _port;
}

void	ClientConnection::clear(){
	_fd = -1;
	_port = 0;
	_ip.clear();
}
