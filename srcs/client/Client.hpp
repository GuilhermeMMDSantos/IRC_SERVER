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

		bool	updaterecvQ(std::string recvBuf);
		int		extractMessage(std::string& line);

		bool	isRegisted() const;
		bool	hasPassword() const;
		bool	hasNick() const;
		bool	hasUser() const;
		void	registreConfirmed();
		void	passwordConfirmed();
		void	nickCreated();
		void	userCreated();
		void	updateNick(const std::string&);
		void	updateUser(const std::string&);
		void	updateRealName(const std::string&);
		int		getFd() const;
		std::string	getNickName() const;
		std::string	getUserName() const;
		std::string	getIP() const;

};

#endif
