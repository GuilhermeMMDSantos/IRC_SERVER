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

	if (!channel){
		server.sendMessage(client, ":"+ server._name + " 403 " + client.getNickName() + " " + chanName + " :No such channel");
		return ;
	}
	if (channel->hasImode() && !channel->wasInvited(client.getNickName())){
		server.sendMessage(client,  ":"+ server._name + " 473 " + client.getNickName() + " " + chanName + " :Invite only channel");
		return ;
	}

	if (channel->isMember(client.getFd())) return ;

	if ( channel->hasPassword() && !channel->checkPassword(password)){
		server.sendMessage(client,  ":"+ server._name + " 475 " + client.getNickName() + " " + chanName + " :Bad channel key");
		return ;
	}

	channel->addMember(client, channel->isFirstMember());
	channel->removerFromInvite(client.getNickName());
	std::string joinMsg = ":" + client.getNickName() + "!~"
						+ client.getUserName() + "@" + client.getIP()
						+ " JOIN " + chanName;

	std::map<int, Client> members = channel->getMembers();
	channel->broadcast(server, client, joinMsg, true);

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
