#include "Executor.hpp"
#include "Server.hpp"

Executor::Executor(){
	_command.clear();
	_command.insert(std::make_pair("PASS", &Executor::pass));
	_command.insert(std::make_pair("NICK", &Executor::nick));
	_command.insert(std::make_pair("JOIN", &Executor::join));
	_command.insert(std::make_pair("USER", &Executor::user));
	_command.insert(std::make_pair("CAP", &Executor::cap));
	_command.insert(std::make_pair("PRIVMSG", &Executor::privmsg));
	_command.insert(std::make_pair("MODE", &Executor::mode));
	_command.insert(std::make_pair("PING", &Executor::mode));
	
}

bool	Executor::isValidName(const std::string& nick){
	if (nick.empty() || nick.size() > 9) return false;
	if (isdigit(nick[0])) return false;
	std::string allowed = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_-\\[]{}^`";
	if (nick.find_first_not_of(allowed) != std::string::npos)
		return false;
	return true;
}

void	Executor::welcome(Server& server, Client& client){
	if (client.isRegisted()) return;
	if (!client.hasNick() || !client.hasUser()) return ;
	if (!server.getPassword().empty() && !client.hasPassword()){
		server.sendMessage(client, ":"+ server._name + " 464 * :Password incorrect");
		server.disconnectClient(client.getFd());
		return;
	}
	client.registreConfirmed();

	std::string nick = client.getNickName().empty() ? "*" : client.getNickName();
	std::string user = client.getUserName().empty() ? "*" : client.getUserName();;
	std::string host = client.getIP();
	std::string rpl001 = ":" + server._name + " 001 " + nick +
						 " :Welcome to the Internet Relay Network " +
						 nick + "!" + user + "@" + host;
	std::string rpl002 = ":" + server._name + " 002 " + nick +
						 " :Your host is " + server._name + ", running version 1.0\r\n";
	std::string rpl003 = ":" + server._name + " 003 " + nick +
						 " :This server is for 42 Avaluation";
	std::string rpl004 = ":" + server._name + " 004 " + nick +
						 " " + server._name + " 1.0 oOitv oOitv";

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
		unknown(server, client, msg);
}

void	Executor::unknown(Server& server, Client& client, const Message& msg){
	std::string nick = client.getNickName().empty() ? "*" : client.getNickName();

	std::string response =":" + server._name + " 421 " + nick + " " + msg.getCommand() + " :Unknown command";
	server.sendMessage(client, response);
}

bool	Executor::verify(Server& server, Client& client){
	if (!client.isRegisted() || !client.hasPassword()){
		server.sendMessage(client, ":"+ server._name + " 462 * :You have not registered (need PASS)");
		return false;
	}
	return true;
}

