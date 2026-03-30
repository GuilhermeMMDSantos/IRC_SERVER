#include "Executor.hpp"
#include "Server.hpp"

void Executor::invite(Server& server, Client& client, const Message& msg) {
	if (!verify(server, client)) return ;
	std::vector<std::string> params = msg.getParams();
	if (params.size() < 2) {
		server.sendMessage(client, ":" + server._name + " 461 " + client.getNickName() + " INVITE :Not enough parameters");
		return;
	}

	std::string targetNick = params[0];
	std::string name = params[1];
	Client* targetClient = server.getClientByNick(targetNick);
	if (!targetClient) {
		server.sendMessage(client, ":" + server._name + " 401 " + client.getNickName() + " " + targetNick + " :No such nick/channel");
		return;
	}
	Channel* channel = server.getChannelByName(name);
	if (!channel) {
		server.sendMessage(client, ":" + server._name + " 403 " + client.getNickName() + " " + name + " :No such channel");
		return;
	}
	if (!channel->isMember(client.getFd())) {
		server.sendMessage(client, ":" + server._name + " 442 " + client.getNickName() + " " + name + " :You're not on that channel");
		return;
	}
	if (channel->isMember(targetClient->getFd())) {
		server.sendMessage(client, ":" + server._name + " 443 " + client.getNickName() + " " + targetNick + " " + name + " :is already on channel");
		return;
	}
	if (channel->hasImode() && !channel->isOperator(client.getFd())) {
		server.sendMessage(client, ":" + server._name + " 482 " + client.getNickName() + " " + name + " :You're not channel operator");
		return;
	}

	server.sendMessage(client, ":" + server._name + " 341 " + client.getNickName() + " " + targetNick + " " + name);


	std::string inviteMsg = ":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getIP() + " INVITE " + targetNick + " :" + name;
	server.sendMessage(*targetClient, inviteMsg);
	channel->addToInviteList(targetNick);
}
