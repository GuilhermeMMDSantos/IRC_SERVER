#include "ClientConnection.hpp"

ClientConnection::ClientConnection(int fd, std::string ip, short port): _fd(fd), _ip(ip), _port(port){}

ClientConnection::ClientConnection(const ClientConnection& src): _fd(src._fd), _ip(src._ip), _port(src._port){}

int			ClientConnection::getfd() const{
	return _fd;
}

std::string	ClientConnection::getIP() const{
	return _ip;
}

short		ClientConnection::getPort() const{
	return _port;
}
