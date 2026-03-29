#ifndef __EXECUTOR_HPP__
#define __EXECUTOR_HPP__

#include <map>
#include <string>
#include <algorithm>
#include "Client.hpp"
#include "Message.hpp"
#include "Mode.hpp"
#include "util/Util.hpp"
#include "Channel.hpp"
#include "ChannelManager.hpp"

class Server;

class Executor
{
	private:
		typedef void (Executor::*_handler)(Server&, Client&, const Message&);
		std::map<std::string, _handler> _command;

		void	welcome(Server&, Client&);
		bool	verify(Server&, Client&);
		void	cap(Server&, Client&, const Message&);
		void	pass(Server&, Client&, const Message&);
		void	nick(Server&, Client&, const Message&);
		void	join(Server&, Client&, const Message&);
		void	user(Server&, Client&, const Message&);
		void	privmsg(Server&, Client&, const Message&);
		void	mode(Server&, Client&, const Message&);
		void	ping(Server&, Client&, const Message&);
		void	topic(Server&, Client&, const Message&);
		void	kick(Server&, Client&, const Message&);
		bool	isValidName(const std::string&);
	public:
		Executor();

		void	execute(Server&, Client&, const Message&);
		void	unknown(Server&, Client&, const Message&);
};

#endif
