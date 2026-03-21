#ifndef __CLIENTCONNECTION_HPP__
#define __CLIENTCONNECTION_HPP__

#include <string>

class ClientConnection
{
	private:
		int			_fd;
		std::string	_ip;
		short		_port;

	public:
		ClientConnection(int fd, std::string ip = "", short port = -1);
		ClientConnection(const ClientConnection& src);
		int			getfd() const;
		std::string	getIP() const;
		short		getPort() const;
};

#endif
