#include "Server.hpp"

Server::Server(const char *port, const std::string& password)
:_socket(port), _password(password), _isRunning(false), _name("ft_irc.42.fr")
{
	_tmpfd = open("/dev/null", O_RDONLY);
	if (_tmpfd < 0){
		throw std::runtime_error("open /dev/null");
	}
}

Server::~Server()
{
	stop();
}

std::string	Server::getPassword() const{
	return _password;
}

void Server::stop()
{
	if (!_isRunning) return;
	_isRunning = 0;
	for (size_t i = 0; i < fds.size(); i++){
		if (fds[i].fd >= 0)
			close(fds[i].fd);
	}
	fds.clear();
	_clientMgr.clear();
	close(_tmpfd);
	close(_socket.getSockFd());
	std::cout << "Server stoped!" << std::endl;
}

void Server::acceptClient(){
	ClientConnection client = _socket.acceptClient();
	int clientfd = client.getfd();
	if (clientfd < 0){
		if (errno == EMFILE || errno == ENFILE){
			close(_tmpfd);
			client.clear();
			client = _socket.acceptClient();
			clientfd = client.getfd();
			if (clientfd >= 0){
				close(clientfd);
			}
			_tmpfd = open("/dev/null", O_RDONLY);
			if (_tmpfd < 0){
				std::cerr << "Error: open /dev/null\n";
				stop();
			}
		} else
			std::cerr << "Warning: accept() failed" << std::endl;
		return;
	}
	pollfd pfd;
	pfd.fd = clientfd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	fds.push_back(pfd);
	_clientMgr.addClient(client);

	std::cout << "[+] New client connected: fd=" << clientfd
	<< " IP=" << client.getIP()
	<< ":" << client.getPort() << std::endl;
}

void Server::disconnectClient(int fd){
	std::cout << "[-] Client disconnected: fd=" << fd << std::endl;
	close(fd);
	_clientMgr.removeClient(fd);
	for (std::vector<pollfd>::iterator it = fds.begin(); it != fds.end(); ++it){
		if (it->fd == fd){
			fds.erase(it);
			break;
		}
	}
}

void Server::handleClient(int fd){

	char buf[BUFFER_SIZE];
	ssize_t bytesRead = recv(fd, buf, sizeof(buf) - 1, 0);
	if (bytesRead <= 0){
		disconnectClient(fd);
		return ;
	}

	buf[bytesRead] = '\0';

	if (!_clientMgr.updaterecvQ(fd, std::string(buf, bytesRead))){
		disconnectClient(fd);
		return;
	}

	std::string message;
	while (_clientMgr.extractMessage(fd, message)){
		if (!message.empty())
			processMessage(fd, message);
	}
}

void Server::processMessage(int fd, const std::string& message){
	std::cout << "[fd=" << fd << "] >>" << message << std::endl;
	Message	msg;
	if (!msg.parse(message)){
		std::cerr << "Warning: command invalid" << std::endl;
		return;
	}
	Client* client = _clientMgr.getClient(fd);
	if (!client){
		std::cerr << "Warning: client not found for fd: " << fd << std::endl;
		return;
	}
	_executor.execute(*this, *client, msg);
	//Tudo abaixo é para teste!! Apartir daqui pode usar a objecto msg (tipo class Message) para executar os comandos.
	// std::cout << "Prefix: " << msg.getPrefix() << std::endl;
	// std::cout << "Command: " << msg.getCommand() << std::endl;
	// std::vector<std::string> params = msg.getParams();
	// std::cout << "Param ";
	// for (size_t i = 0; i <	params.size(); ++i)
	// 	std::cout << i << ":[" << params[i] << " ] ";
	// std::cout << "\n";
}

void Server::start(){

	try
	{
		_socket.start();
		int sockfd = _socket.getSockFd();

		pollfd pfd;
		pfd.fd = sockfd;
		pfd.events = POLLIN;
		pfd.revents = 0;
		fds.push_back(pfd);

		_isRunning = true;
		std::cout << "=== IRC Server started ===" << std::endl;
		std::cout << "Port: " << _socket.getPort() << std::endl;
		std::cout << "Press Ctrl+C to stop\n" << std::endl;
		while (_isRunning)
		{
			int ready = poll(fds.data(), fds.size(), -1);
			if (ready < 0 ){
				if (errno == EINTR) continue;
				std::cerr << "Warning: poll interrupted" << std::endl;
				continue;
			}

			size_t currentSize = fds.size();
			for (size_t i = 0; i < currentSize; ++i){
				if (fds[i].revents == 0)
					continue;
				if (fds[i].fd == sockfd)
					acceptClient();
				else
					handleClient(fds[i].fd);
			}
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		stop();
	}
}

void Server::sendMessage(Client& client, const std::string& msg){
	std::string message = msg + "\r\n";
	const char* buffer = message.c_str();
	size_t total = message.size();
	size_t sent = 0;
	while (sent < total){
		ssize_t bytes = send(client.getFd(), buffer + sent, total - sent, 0);
		if (bytes <= 0){
			std::cerr << "send() falied\n";
			break ;
		}
		sent += bytes;
	}
}

bool	Server::isNickAvailable(const std::string& nick) {
	return _clientMgr.isNickUnique(nick);
}

Client*	Server::getClientByNick(const std::string& nick){
	return _clientMgr.getByNick(nick);
}

Channel*	Server::getChannelByName(const std::string& name){
 	return _channelMgr.getChannel(name);
}

std::string	Server::activesModes(const std::string& channel){
	std::string ch = "";
	Channel *target = getChannelByName(channel);
	if (!target) return ch;
	ch = (*target).getActivesModes();
	return ch;
}
Channel* Server::getOrCreateChannel(const std::string& name){
	return _channelMgr.getOrCreate(name);
}
