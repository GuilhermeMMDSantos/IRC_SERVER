#include "Executor.hpp"
#include "Server.hpp"

Executor::Executor(){
	_command.clear();
	_command.insert(std::make_pair("PASS", &Executor::pass));
	_command.insert(std::make_pair("NICK", &Executor::nick));
	_command.insert(std::make_pair("USER", &Executor::user));
}


void	Executor::pass(Server& server, Client& client, const Message& msg){
	std::string pwd = server.getPassword();
	if (msg.getParams().size() < 1){
		server.sendMessage(client, ":ircserv 461 * PASS :Not enough parameters");
		return ;
	}
	if (client.hasPassword()){
		if (client.isRegisted())
			server.sendMessage(client, ":ircserv 462 * :Already registed");
		return;
	}
	if (pwd == msg.getParams()[0])
		client.passwordConfirmed();
	else
		server.sendMessage(client, ":ircserv 464 * :Password incorrect");
}

bool	Executor::isValidName(const std::string& nick){
	if (nick.empty() || nick.size() > 9) return false;
	if (isdigit(nick[0])) return false;
	std::string allowed = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_-\\[]{}^`";
	if (nick.find_first_not_of(allowed) != std::string::npos)
		return false;
	return true;
}

void	Executor::nick(Server& server, Client& client, const Message& msg){
	if (client.isRegisted()){
		server.sendMessage(client, ":ircserv 462 * :You have not registered (need PASS)");
		return;
	}
	if (msg.getParams().empty()){
		server.sendMessage(client, ":ircserv 431 * NICK :No nickname given");
		return ;
	}
	if (msg.getParams().size() > 1){
		server.sendMessage(client, ":ircserv 461 * NICK :Not enough parameters");
		return ;
	}
	std::string m = msg.getParams()[0];
	if (!isValidName(m)){
		server.sendMessage(client, ":ircserv 432 * NICK :" + m +" Erroneous Nickname");
		return ;
	}
	if (!server.isNickAvailable(m)){
		server.sendMessage(client, ":ircserv 433 * NICK :Nickname is already in use");
		return ;
	}
	client.updateNick(m);
	client.nickCreated();
}

void	Executor::user(Server& server, Client& client, const Message& msg){
	if (client.isRegisted()){
		server.sendMessage(client, ":ircserv 462 * :You have not registered (need PASS)");
		return;
	}
	if (msg.getParams().size() < 4){
		server.sendMessage(client, ":ircserv 461 * USER :Not enough parameters");
		return ;
	}
	std::string m = msg.getParams()[0];
	if (!isValidName(m)){
		server.sendMessage(client, ":ircserv 432 * USER :" + m +" Erroneous Nickname");
		return ;
	}
	client.updateUser(m);
	client.updateRealName(msg.getParams()[3]);
	client.userCreated();
	welcome(server, client);
}

void	Executor::welcome(Server& server, Client& client){
	if (client.isRegisted()) return;
	if (!client.hasNick() && !client.hasUser()) return ;
	if (!server.getPassword().empty() && !client.hasPassword()){
		server.sendMessage(client, ":ircserv 464 * :Password incorrect");
		server.disconnectClient(client.getFd());
		return;
	}
	client.registreConfirmed();

	std::string srvName = "ircserv";
	std::string nick = client.getNickName();
	std::string user = client.getUserName();
	std::string host = client.getIP();
	std::string rpl001 = ":" + srvName + " 001 " + nick +
						 " :Welcome to the Internet Relay Network " +
						 nick + "!" + user + "@" + host;
	std::string rpl002 = ":" + srvName + " 002 " + nick +
						 " :Your host is " + srvName + ", running version 1.0\r\n";
	std::string rpl003 = ":" + srvName + " 003 " + nick +
						 " :This server is for 42 Avaluation";
	std::string rpl004 = ":" + srvName + " 004 " + nick +
						 " " + srvName + " 1.0 oOitv oOitv";

	server.sendMessage(client, rpl001);
	server.sendMessage(client, rpl002);
	server.sendMessage(client, rpl003);
	server.sendMessage(client, rpl004);

	std::cout << "[+] Client " << nick << " (" << client.getFd() << ") is now fully registered!" << std::endl;
}

void	Executor::execute(Server& server, Client& client, const Message& msg){
	std::map<std::string, _handler>::iterator it;
	std::string cmdstr = msg.getCommand();

	std::transform(cmdstr.begin(), cmdstr.end(), cmdstr.begin(), ::toupper);
	it = _command.find(cmdstr);
	if (it != _command.end())
		(this->*(it->second))(server, client, msg);
	else
		unknown();
}

void	Executor::unknown(){

}

