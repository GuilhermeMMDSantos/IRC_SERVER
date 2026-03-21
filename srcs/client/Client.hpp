#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

#include <string>
#include "ClientIdentity.hpp"
#include "ClientConnection.hpp"
#include "ClientState.hpp"

class Client
{
	private:
		ClientConnection	_connection;
		std::string			_recvQ;
		ClientIdentity		_identity;
		ClientState			_states;

	public:
		Client(const ClientConnection& connection, std::string recvQ = "");

		void	updaterecvQ(std::string recvBuf);
		bool	extractMessage(std::string& line);
};

#endif
