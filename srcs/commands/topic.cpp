#include "Executor.hpp"
#include "Server.hpp"

void Executor::topic(Server& server, Client& client, const Message& msg) {
	if (!verify(server, client)) return ;
	std::vector<std::string> params = msg.getParams();
	if (params.empty()) {
		server.sendMessage(client, ":"+ server._name + " 461 * NICK :Not enough parameters");
		return;
	}

	std::string name = params[0];
	Channel* channel = server.getChannelByName(name);
	if (!channel) {
		server.sendMessage(client, ":" + server._name + " 403 " + client.getNickName() + " " + name + " :No such channel");
		return;
	}
	if (params.size() == 1) {
		std::string curTopic = channel->getTopic();
		if (curTopic.empty()) {
			server.sendMessage(client, ":" + server._name + " 331 " + client.getNickName() + " " + name + " :No topic is set");
		} else {
			server.sendMessage(client, ":" + server._name + " 332 " + client.getNickName() + " " + name + " :" + channel->getTopic());
		}
		return;
	}
	if (channel->hasTmode() && !channel->isOperator(client.getFd())) {
		server.sendMessage(client, ":" + server._name + " 482 " + client.getNickName() + " " + name + " :You're not channel operator");
		return;
	}

	std::string newTopic = params[1];
	channel->setTopic(newTopic);
	std::string response = ":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getIP() + " TOPIC " + name + " :" + newTopic;
	channel->broadcast(server, client, response, true);
}
