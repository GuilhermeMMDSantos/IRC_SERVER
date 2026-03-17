#include "Server.hpp"

Server::Server(const std::string& host, int port): serverSocket(-1), port(port),
host(host), isRunning(false)
{
	std::memset(&serverAddr, 0, sizeof(serverAddr));
}

Server::~Server()
{
	stop();
}

void Server::stop()
{
	if (serverSocket >= 0)
	{
		close(serverSocket);
		serverSocket = -1;
	}
	isRunning = false;
	std::cout << "Web server stop!" << std::endl;
}

void Server::setupSocket()
{
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0)
		throw std::runtime_error("Error: Can't create socket");
	int opt = 1;
	if (setsockopt(serverSocket, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt)) < 0)
	{
		close(serverSocket);
		throw std::runtime_error("Error: Can't config socket");
	}
}

void  Server::bindSocket(){

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);

	if (host == "0.0.0.0" || host.empty())
		serverAddr.sin_addr.s_addr = INADDR_ANY;
	else
	{
		try{
			serverAddr.sin_addr.s_addr = stringToIpv4(host);
		}catch(std::exception& e){
			close(serverSocket);
			throw std::runtime_error(e.what());
		}
	}

	if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
	{
		close(serverSocket);
		throw std::runtime_error("Error: Can't bind socket to IP&Port");
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

void Server::handleConnection(int clientSocket)
{
	char buffer[BUFFER_SIZE];
	std::memset(buffer, 0, BUFFER_SIZE);
	ssize_t bytesRead = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
	if (bytesRead < 0)
		throw std::runtime_error("Error: Cant't read data in socket");
	if (bytesRead == 0)
		return ;
	std::string requestLine(buffer, bytesRead);

	std::cout << "\n --------------- REQUEST----------" << std::endl;
	std::cout << requestLine << std::endl;
	std::cout << "\n ---------------------------------" << std::endl;

	std::string method;
	std::string uri;
	std::string bodyRequest;
	std::map<std::string, std::string> headers;
	bodyRequest = parseRequest(requestLine, method, uri, headers);

	std::cout << "Method: " << method << std::endl;
	std::cout << "URI: " << uri << std::endl;
	std::cout << "Headers: " << std::endl;
	for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
		std::cout << " " << it->first <<": " << it->second << std::endl;
	if (!bodyRequest.empty())
		std::cout << "Body: " << bodyRequest << std::endl;
	std::string response = "HTTP 1.0 200 Ok\r\n";
	response+= "Content-Type: text/html\r\n";
	response+= "Connection: close\r\n";
	response+= "\r\n";
	response+= "<html><header><title>webserver</title></header><body><h1>ESTÁ VIVO!</h1></body></html>";

	send(clientSocket, response.c_str(), response.length(), 0);
}



void Server::start(){

	try
	{
		setupSocket();
		bindSocket();
		startListening();

		isRunning = true;
		std::cout << "\n=== HTTP Server started ===" << std::endl;
		std::cout << "Addr: http://" << host << ":" << port << std::endl;
		std::cout << "Press Ctrl+c to stop\n" << std::endl;

		while (isRunning)
		{
			sockaddr_in clientAddr;
			socklen_t clientLen = sizeof(clientAddr);

			int clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientLen);

			if (clientSocket < 0)
			{
				std::cerr << "Error: cant't accept connection" << std::endl;
				continue;
			}

			unsigned int rawIp = clientAddr.sin_addr.s_addr;
			unsigned int hostIp = ntohl(rawIp);
			std::string clientIP = ipv4ToString(hostIp);
			std::cout << "\n[New connection] Client: " << clientIP << ":" << ntohs(clientAddr.sin_port) << std::endl;

			handleConnection(clientSocket);
			close(clientSocket);
			std::cout << "[Connection clesed] Client: " << clientIP << std::endl;
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		stop();
	}
}



//PARSE DA REQUEST FICA NO OUTRO OBJETO
std::string Server::parseRequest(const std::string& requestLine, std::string& method, std::string& uri, std::map<std::string, std::string>& headers)
{

	std::istringstream stream(requestLine);
	std::string line;
	if (std::getline(stream, line)){
		if(!line.empty() && line[line.length() - 1] == '\r')
			line = line.substr(0, line.length() - 1);

		std::istringstream lineStream(line);
		std::string httpVersion;
		lineStream >> method >> uri >> httpVersion;
	}

	while (std::getline(stream, line)){
		if(!line.empty() && line[line.length() - 1] == '\r')
			line = line.substr(0, line.length() - 1);
		if (line.empty())
			break;
		size_t separatorPos = line.find(':');
		if ( separatorPos != std::string::npos)
		{
			std::string key = line.substr(0, separatorPos);
			std::string value = line.substr(separatorPos + 1);
			size_t firstLetterPos = value.find_first_not_of(" \t");
			if (firstLetterPos != std::string::npos)
				value = value.substr(firstLetterPos);
			headers[key] = value;
		}
	}

	std::string bodyRequest = "";
	while (std::getline(stream, line)){
		bodyRequest += line + "\n";
	}
	return bodyRequest;
}
