#include "Executor.hpp"
#include "Server.hpp"

void	Executor::user(Server& server, Client& client, const Message& msg){
	if (client.isRegisted()){
		server.sendMessage(client, ":"+ server._name + " 462 * :Already registed)");
	}
	//if (!client.hasNick()){
	//	server.sendMessage(client, ":"+ server._name + " 462 * :You have not registered (need NICK)");
	//	return;
	//}
	if (msg.getParams().size() < 4){
		server.sendMessage(client, ":"+ server._name + " 461 * USER :Not enough parameters");
		return ;
	}
	std::string m = msg.getParams()[0];
	if (!isValidName(m)){
		server.sendMessage(client,":"+ server._name + " 432 * USER :" + m +" Erroneous Nickname");
		return ;
	}
	client.updateUser(m);
	client.updateRealName(msg.getParams()[3]);
	client.userCreated();
	if (client.hasPassword() && client.hasNick())
		welcome(server, client);
}
