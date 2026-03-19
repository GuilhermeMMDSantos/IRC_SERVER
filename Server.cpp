#include "Server.hpp"

Server::Server(int port, const std::string& password)
: serverSocket(-1), port(port), password(password), isRunning(false)
{
	std::memset(&serverAddr, 0, sizeof(serverAddr));
}

Server::~Server()
{
	stop();
}

void Server::stop()
{
	isRunning = false;
	for (size_t i = 0; i < fds.size(); i++){
		if (fds[i].fd >= 0)
			close(fds[i].fd);
	}
	fds.clear();
	recvBuf.clear();
	serverSocket = -1;
	std::cout << "Server stoped!" << std::endl;
}

void Server::setNonBlocking(int fd){
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags < 0)
		throw std::runtime_error("Error: fcntl F_GETFL failed");
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
		throw std::runtime_error("Error: fcntl F_SETFL O_NONBLOCK failed");
}

void Server::setupSocket()
{
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0)
		throw std::runtime_error("Error: Can't create socket");
	int opt = 1;
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		close(serverSocket);
		throw std::runtime_error("Error: Can't config socket");
	}
	setNonBlocking(serverSocket);
}

void  Server::bindSocket(){

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
	{
		close(serverSocket);
		throw std::runtime_error("Error: Can't bind socket");
	}
}

void Server::startListening()
{
	if (listen(serverSocket, MAX_CONNECTIONS) < 0)
	{
		close(serverSocket);
		throw std::runtime_error("Error: Server can't listening");
	}
}

void Server::acceptClient(){
	sockaddr_in clientAddr;
	socklen_t clientlen = sizeof(clientAddr);

	int clientFd = accept(serverSocket, (sockaddr*)&clientAddr, &clientlen);
	if (clientFd < 0){
		std::cerr << "Warning: accept() failed" << std::endl;
		return;
	}
	setNonBlocking(clientFd);
	pollfd pfd;
	pfd.fd = clientFd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	fds.push_back(pfd);
	recvBuf[clientFd] = "";

	std::cout << "[+] New client connected: fd=" << clientFd 
	<< " IP=" << inet_ntoa(clientAddr.sin_addr)
	<< ":" << ntohs(clientAddr.sin_port) << std::endl; 
}

void Server::disconnectClient(int fd){
	std::cout << "[-] Client disconnected: fd=" << fd << std::endl;
	close(fd);
	recvBuf.erase(fd);
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
	recvBuf[fd] += std::string(buf, bytesRead);
	std::string& accumulated = recvBuf[fd];
	size_t pos;
	while ( (pos = accumulated.find('\n')) != std::string::npos ){
		std::string message = accumulated.substr(0, pos);
		accumulated = accumulated.substr(pos + 1);

		if (!message.empty() && message[message.size() - 1] == '\r')
			message.erase(message.size() - 1);
		if (!message.empty())
			processMessage(fd, message);
	}
}

void Server::processMessage(int fd, const std::string& message){
	std::cout << "[fd=" << fd << "] >>" << message << std::endl;
	//Aqui onde vai ser feito o parse da mensagem para extrairmos os comandos e companhia!
	//Depois vamos criar logica para cada comando
}

void Server::start(){

	try
	{
		setupSocket();
		bindSocket();
		startListening();

		pollfd serverPfd;
		serverPfd.fd = serverSocket;
		serverPfd.events = POLLIN;
		serverPfd.revents = 0;
		fds.push_back(serverPfd);

		isRunning = true;
		std::cout << "=== IRC Server started ===" << std::endl;
		std::cout << "Port: " << port << std::endl;
		std::cout << "Press Ctrl+C to stop\n" << std::endl;
		while (isRunning)
		{
			int ready = poll(fds.data(), fds.size(), -1);
			if (ready < 0){
				std::cerr << "Warning: poll interrupted" << std::endl;
				continue;
			}

			size_t currentSize = fds.size();
			for (size_t i = 0; i < currentSize; ++i){
				if (fds[i].revents == 0)
					continue;
				if (fds[i].fd == serverSocket)
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