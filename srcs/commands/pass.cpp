#include "Executor.hpp"
#include "Server.hpp"

void	Executor::pass(Server& server, Client& client, const Message& msg){
	std::string pwd = server.getPassword();
	if (msg.getParams().size() < 1){
		server.sendMessage(client, ":"+ server._name + " 461 * PASS :Not enough parameters");
		return ;
	}
	if (client.hasPassword()){
		if (client.isRegisted())
			server.sendMessage(client, ":"+ server._name + " 462 * :Already registed");
		return;
	}
	if (pwd == msg.getParams()[0])
		client.passwordConfirmed();
	else
		server.sendMessage(client, ":"+ server._name + " 464 * :Password incorrect");
}
