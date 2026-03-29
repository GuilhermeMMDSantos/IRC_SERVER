#include "Executor.hpp"
#include "Server.hpp"

void	Executor::privmsg(Server& server, Client& client, const Message& msg){
	if (!verify(server, client)) return ;
	std::vector<std::string> params = msg.getParams();
	if (params.empty()){
		server.sendMessage(client, ":"+ server._name + " 411 :No recipient given (PRIVMSG)");
		return;
	}
	if (params.size() < 2 || params[1].empty()){
		server.sendMessage(client, ":"+ server._name + " 412 :No text to send");
		return;
	}

	std::string target = params[0];
	std::string text = params[1];
	// quando a mensagem é para o canal
	if (target[0] == '#' || target[0] == '&'){

	} else {
		Client* targetClient = server.getClientByNick(target);
		if (!targetClient){

			server.sendMessage(client, ":"+ server._name + " 401 " + target + " :No such nick");
			return;
		}
		std::string message = ":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getIP() + " PRIVMSG " + target + " :" + text;
		server.sendMessage(*targetClient, message);
	}
}
