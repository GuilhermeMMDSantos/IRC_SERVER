#ifndef SERVER_H
#define SERVER_H

#define MAX_CONNECTIONS 1000
#define BUFFER_SIZE 4046

#include <string>
#include <cstdlib>
#include <map>
#include <iostream>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sstream>
#include <csignal>
#include "utils.hpp"

class Server{
	private:
		int serverSocket;
		int port;
		std::string host;
		sockaddr_in serverAddr;
		bool isRunning;

		void setupSocket();
		void bindSocket();
		void startListening();
		std::string parseRequest(const std::string& requestLine, std::string& method, std::string& uri, std::map<std::string, std::string>& headers);

	public:

		Server(const std::string& host, int port);
		~Server();

		void start();
		void stop();
		void handleConnection(int clientSocket);
};
#endif
