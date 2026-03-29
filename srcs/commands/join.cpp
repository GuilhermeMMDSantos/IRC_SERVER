#include "Executor.hpp"
#include "Server.hpp"

void Executor::join(Server& server, Client& client, const Message& msg){

	if (!verify(server, client)) return ;

	const std::vector<std::string>& params = msg.getParams();
	if (params.empty())
		return server.sendMessage(client, ":"+ server._name + " 461 JOIN :Not enough parameters");

	const std::string& chanName = params[0];
	std::string password = (params.size() > 1) ? params[1] : "";

	Channel* channel = server.getOrCreateChannel(chanName);

	if (!channel)
		return server.sendMessage(client, ":"+ server._name + " 403 " + client.getNickName() + " " + chanName + " :No such channel");

	if (channel->isMember(client.getFd())) return ;

	if ( channel->hasPassword() && !channel->checkPassword(password))
		return server.sendMessage(client,  ":"+ server._name + " 475 " + client.getNickName() + " " + chanName + " :Bad channel key");

	channel->addMember(client, channel->isFirstMember());

	std::string joinMsg = ":" + client.getNickName() + "!~"
						+ client.getUserName() + "@" + client.getIP()
						+ " JOIN " + chanName;

	std::map<int, Client> members = channel->getMembers();

	// Fazer um broadcast e tirar este loop
	for (std::map<int, Client>::iterator it = members.begin(); it != members.end(); ++it)
		server.sendMessage(it->second, joinMsg);

	if (!channel->getTopic().empty())
		server.sendMessage(client, ":" + server._name +" 332 " + client.getNickName()
		+ " " + chanName + " :" + channel->getTopic());
	else
		server.sendMessage(client, ":" + server._name +" 331 " + client.getNickName()
		+ " " + chanName + " :No topic is set");

	server.sendMessage(client, ":" + server._name + " 353 " + client.getNickName()
		+ " = " + chanName + " :" + channel->getMemberList());

	server.sendMessage(client, ":" + server._name + " 366 " + client.getNickName()
		+ " " + chanName + " :End of /NAMES list");
}
