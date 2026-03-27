#ifndef __CLIENTMANAGER_HPP__
#define __CLIENTMANAGER_HPP__

#include <string>
#include <map>
#include "Client.hpp"
class ClientManager
{
	private:
		std::map<int, Client>	_clients;

		bool	comp(const std::string&, const std::string&) const;
	public:
		void	addClient(const ClientConnection& connection, std::string recvQ = "");
		void	removeClient(int fd);
		bool	updaterecvQ(int fd, std::string recvBuf);
		bool	extractMessage(int fd, std::string& line);
		Client*	getClient(int fd);
		Client*	getByNick(const std::string& nick);
		std::map<int, Client>& getAllClient();
		void	clear();
		bool	isNickUnique(const std::string&) const;
};

#endif
