#include "Executor.hpp"
#include "Server.hpp"

void Executor::kick(Server& server, Client& client, const Message& msg) {
	if (!verify(server, client)) return ;
	std::vector<std::string> params = msg.getParams();

	if (params.size() < 2) {
		server.sendMessage(client, ":" + server._name + " 461 " + client.getNickName() + " KICK :Not enough parameters");
		return;
	}

	std::string name = params[0];
	std::string targetNick = params[1];

	std::string reason = client.getNickName();
	if (params.size() > 2 && !params[2].empty()) {
		reason = params[2];
	}

	Channel* channel = server.getChannelByName(name);
	if (!channel) {
		server.sendMessage(client, ":" + server._name + " 403 " + client.getNickName() + " " + name + " :No such channel");
		return;
	}
	if (!channel->isOperator(client.getFd())) {
		server.sendMessage(client, ":" + server._name + " 482 " + client.getNickName() + " " + name + " :You're not channel operator");
		return;
	}

	Client* targetClient = channel->getClientByNick(targetNick);
	if (!targetClient || !channel->isMember(targetClient->getFd())) {
		server.sendMessage(client, ":" + server._name + " 441 " + client.getNickName() + " " + targetNick + " " + name + " :They aren't on that channel");
		return;
	}

	std::string kickMsg = ":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getIP() + " KICK " + name + " " + targetNick + " :" + reason;
	channel->broadcast(server, client, kickMsg, true);
	channel->removeMember(targetClient->getFd());
	if (channel->isEmpty()){
		server.removeChannel(name);
	}
}
