#ifndef __CLIENTCONNECTION_HPP__
#define __CLIENTCONNECTION_HPP__

#include <string>
#include <stdint.h>

class ClientConnection
{
	private:
		int			_fd;
		std::string	_ip;
		uint16_t	_port;

	public:
		ClientConnection(int fd, std::string ip = "", uint16_t port = 0);
		ClientConnection(const ClientConnection& src);
		int			getfd() const;
		std::string	getIP() const;
		uint16_t	getPort() const;

		void	clear();
};

#endif
