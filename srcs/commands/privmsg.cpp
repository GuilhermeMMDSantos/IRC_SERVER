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
	if (target[0] == '#' || target[0] == '&'){
		Channel* chan = server.getChannelByName(target);
		if (chan) {
			if (!chan->isMember(client.getFd())){
				server.sendMessage(client, ":"+ server._name + " 442 " + client.getNickName()+ " " + target +  ":You're not on that channel");
				return;
			}
			std::string formatted = ":" + client.getNickName() + " PRIVMSG " + target + " :" + text;
			chan->broadcast(server, client, formatted, false);
		}
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
