#ifndef SERVER_H
#define SERVER_H

#define MAX_CONNECTIONS 128
#define BUFFER_SIZE 4096

#include <string>
#include <map>
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <fcntl.h>
#include <poll.h>

class Server{
	private:
		int serverSocket;
		int port;
		std::string password;
		sockaddr_in serverAddr;
		bool isRunning;
		std::vector<pollfd> fds;
		std::map<int, std::string> recvBuf;

		void setupSocket();
		void bindSocket();
		void startListening();
		void setNonBlocking(int fd);
		void acceptClient();
		void handleClient(int fd);
		void disconnectClient(int fd);
		void processMessage(int fd, const std::string& message);
	public:

		Server(int port, const std::string& password);
		~Server();

		void start();
		void stop();
};
#endif
