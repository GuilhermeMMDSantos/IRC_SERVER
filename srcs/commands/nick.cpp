#include "Executor.hpp"
#include "Server.hpp"

void	Executor::nick(Server& server, Client& client, const Message& msg){
	if (!client.hasPassword()){
		server.sendMessage(client, ":"+ server._name + " 462 * :You have not registered (need PASS)");
	}
	if (msg.getParams().empty()){
		server.sendMessage(client, ":"+ server._name + " 431 * NICK :No nickname given");
		return ;
	}
	if (msg.getParams().size() > 1){
		server.sendMessage(client, ":"+ server._name + " 461 * NICK :Not enough parameters");
		return ;
	}
	std::string m = msg.getParams()[0];
	if (!isValidName(m)){
		server.sendMessage(client, ":"+ server._name + " 432 * NICK :" + m +" Erroneous Nickname");
		return ;
	}
	if (!server.isNickAvailable(m)){
		server.sendMessage(client, ":"+ server._name + " 433 * NICK :Nickname is already in use");
		return ;
	}
	client.updateNick(m);
	client.nickCreated();
	if (client.isRegisted()){
		server.sendMessage(client, ":"+ server._name + " :You're now known as " + m);
	}
}
