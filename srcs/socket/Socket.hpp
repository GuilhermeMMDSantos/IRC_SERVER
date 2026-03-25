#ifndef __SOCKET_HPP__
#define __SOCKET_HPP__

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sstream>
#include "ClientConnection.hpp"

#define MAX_CONNECTIONS 128

class Socket
{
	private:
		struct addrinfo	*_servinfo;
		const char 		*_port;
		int				_sockfd;

		void	getAddrInfo();
		void	setNonBlocking(int fd) const;
		void	setupSocket();
		void	bindScoket() const;
		void	startListen() const;
	public:
		Socket(const char *port);
		~Socket(void);

		void				start(void);
		int					getSockFd(void) const;
		ClientConnection	acceptClient() const;
		const char*			getPort() const;
};

#endif
