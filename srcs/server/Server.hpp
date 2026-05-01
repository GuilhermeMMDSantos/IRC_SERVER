#ifndef __SERVER_H__
#define __SERVER_H__

#define MAX_CONNECTIONS 128
#define BUFFER_SIZE 4096

#include <cerrno>
#include <fstream>
#include <vector>
#include <poll.h>
#include "Socket.hpp"
#include "ClientManager.hpp"
#include "ChannelManager.hpp"
#include "Message.hpp"
#include "Executor.hpp"
#include "Channel.hpp"

class Server{
	private:
		Socket								_socket;
		ClientManager						_clientMgr;
		Executor							_executor;
		const std::string					_password;
		bool								_isRunning;
		std::vector<pollfd>					fds;
		std::ifstream						_tmpfd;
		int									_maxClients;
		ChannelManager						_channelMgr;


		void acceptClient();
		void handleClient(int fd);
		void handlerSendClient(int);
		void processMessage(int fd, const std::string& message);
	public:
		const std::string	_name;
		Server(const char* port, const std::string& password);
		~Server();

		void	start();
		void	stop();
		void	detectMaxClients();
		void	disconnectClient(int fd);

		std::string	getPassword() const;
		std::string	activesModes(const std::string&);
		Channel*	getChannelByName(const std::string&);
		bool		isNickAvailable(const std::string&);

		void	sendMessage(Client&, const std::string&);
		Client*	getClientByNick(const std::string& nick);
		Channel* getOrCreateChannel(const std::string& name);
		void	removeChannel(const std::string& name);

};
#endif
