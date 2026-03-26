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
#include "Executor.hpp"

class Server{
	private:
		Socket				_socket;
		ClientManager		_clientMgr;
		Executor			_executor;
		const std::string	_password;
		bool				_isRunning;
		std::vector<pollfd>	fds;
		int					_tmpfd;


		void acceptClient();
		void handleClient(int fd);
		void processMessage(int fd, const std::string& message);
	public:
		const std::string	_name;
		Server(const char* port, const std::string& password);
		~Server();

		void	start();
		void	stop();
		void	disconnectClient(int fd);

		std::string	getPassword() const;
		bool		isNickAvailable(const std::string&) const;

		void	sendMessage(Client&, const std::string&);
		Client*	getClientByNick(const std::string& nick);
};
#endif
