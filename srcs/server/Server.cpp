#include "Server.hpp"

Server::Server(const char *port, const std::string& password)
:_socket(port), password(password), isRunning(false)
{

}

Server::~Server()
{
	stop();
}

void Server::stop()
{
	if (!isRunning) return;
	isRunning = 0;
	for (size_t i = 0; i < fds.size(); i++){
		if (fds[i].fd >= 0)
			close(fds[i].fd);
	}
	fds.clear();
	_clientMgr.clear();
	close(_socket.getSockFd());
	std::cout << "Server stoped!" << std::endl;
}

void Server::acceptClient(){
	ClientConnection client = _socket.acceptClient();
	int clientfd = client.getfd();
	if (clientfd < 0){
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
	_clientMgr.updaterecvQ(fd, std::string(buf, bytesRead));

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
		msg.clear();
		std::cerr << "Warning: command invalid" << std::endl;
		return;
	}

	//Tudo abaixo é para teste!! Apartir daqui pode usar a objecto msg (tipo class Message) para executar os comandos. 
	std::cout << "Prefix: " << msg.getPrefix() << std::endl;
	std::cout << "Command: " << msg.getCommand() << std::endl;
	std::vector<std::string> params = msg.getParams();
	std::cout << "Param ";
	for (size_t i = 0; i <	params.size(); ++i)
		std::cout << i << ":[" << params[i] << " ] ";
	std::cout << "\n";
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

		isRunning = true;
		std::cout << "=== IRC Server started ===" << std::endl;
		std::cout << "Port: " << _socket.getPort() << std::endl;
		std::cout << "Press Ctrl+C to stop\n" << std::endl;
		while (isRunning)
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
