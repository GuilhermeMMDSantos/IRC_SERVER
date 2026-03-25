#ifndef __EXECUTOR_HPP__
#define __EXECUTOR_HPP__

#include <map>
#include <string>
#include <algorithm>
#include "Client.hpp"
#include "Message.hpp"

class Server;

class Executor
{
	private:
		typedef void (Executor::*_handler)(Server&, Client&, const Message&);
		std::map<std::string, _handler> _command;

		void	welcome(Server&, Client&);
		void	pass(Server&, Client&, const Message&);
		void	nick(Server&, Client&, const Message&);
		void	user(Server&, Client&, const Message&);
		bool	isValidName(const std::string&);
	public:
		Executor();

		void	execute(Server&, Client&, const Message&);
		void	unknown();
};

#endif
