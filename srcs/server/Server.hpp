#ifndef SERVER_H
#define SERVER_H

#define MAX_CONNECTIONS 128
#define BUFFER_SIZE 4096

#include <cerrno>
#include <vector>
#include <poll.h>
#include "Socket.hpp"
#include "ClientManager.hpp"
#include "Message.hpp"

class Server{
	private:
		Socket				_socket;
		ClientManager		_clientMgr;
		const std::string	password;
		bool				isRunning;
		std::vector<pollfd>	fds;

		void acceptClient();
		void handleClient(int fd);
		void disconnectClient(int fd);
		void processMessage(int fd, const std::string& message);
	public:

		Server(const char* port, const std::string& password);
		~Server();

		void start();
		void stop();
};
#endif
