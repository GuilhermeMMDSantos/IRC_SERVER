#include "Executor.hpp"
#include "Server.hpp"

void	Executor::mode(Server& server, Client& client, const Message& msg){
	if (!verify(server, client)) return ;
	std::vector<std::string> params = msg.getParams();
	std::string canal = params[0].empty() ? "": params[0];
	if (params.size() < 2){
		std::string res = "";
		if (canal.empty())
			res = ":"+ server._name + " 324 " + client.getNickName();
		else{
			res = ":"+ server._name + " 324 " + client.getNickName() + " " + canal + " " + \
					server.activesModes(canal);
		}
		server.sendMessage(client, res);
		return ;
	}
	if (canal[0] != '#'){
		server.sendMessage(client, ":"+ server._name + " 221 " + client.getNickName());
		return;
	}
	Channel* channel = server.getChannelByName(canal);
	if (!channel){
		server.sendMessage(client, ":"+ server._name + " 403 " + client.getNickName() + " " + \
							canal + " :No such channel");
		return ;
	}

	bool nickFounded;
	bool adding			= true;
	std::string modes	= params[1];
	size_t paramIdx		= 2;
	for (size_t i = 0; i < modes.size(); ++i){
		char c = modes[i];
		if (c == '+'){
			adding = true;
			continue;
		}
		if (c == '-'){
			adding = false;
			continue;
		}
		if (!channel->isOperator(client.getFd())){
			server.sendMessage(client, ":"+ server._name + " 482 " + client.getNickName() + " " + canal + " :You're not channel operator");
			break ;
		}
		std::string targetNick;
		std::string response;
		switch (c)
		{
		case 'i':
			channel->changeImode(adding);
			response = ":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getIP() + " MODE " + canal + " " + (adding ? "+" : "-") + c;
			channel->broadcast(server, client, response, true);
			break;
		case 't':
			channel->changeTmode(adding);
			response = ":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getIP() + " MODE " + canal + " " + (adding ? "+" : "-") + c;
			channel->broadcast(server, client, response, true);
			break;
		case 'k':
			if (adding){
				if (paramIdx >= params.size()){
					server.sendMessage(client, ":"+ server._name + " 461 " + client.getNickName() + " MODE :Not enough parameters");
					break;
				}
				channel->changeKmode(true, params[paramIdx++]);
			} else
				channel->changeKmode(false, "");
			response = ":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getIP() + " MODE " + canal + " " + (adding ? "+" : "-") + c + " " + (adding ? params[paramIdx - 1] : "");
			channel->broadcast(server, client, response, true);
			break;
		case 'o':
			if (paramIdx >= params.size()){
				server.sendMessage(client, ":"+ server._name + " 461 " + client.getNickName() + \
									" MODE :Not enough parameters");
				break ;
			}
			targetNick = params[paramIdx++];
			if (!adding) {nickFounded = channel->removeOperator(targetNick);}
			else {nickFounded = channel->addOperator(targetNick);}
			if (!nickFounded){
				server.sendMessage(client, ":" + server._name + " 441 " + client.getNickName() + " " + targetNick + " " + canal + " :They aren't on that channel");
				break;
			}
			response = ":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getIP() + " MODE " + canal + " " + (adding ? "+" : "-") + c + " " + targetNick;
			channel->broadcast(server, client, response, true);
			break;
		case 'l':
			if (adding){
				if ( paramIdx >= params.size() || !Util::isValidNumber(params[paramIdx])){
					server.sendMessage(client, ":"+ server._name + " 461 " + client.getNickName() + \
										" MODE :Not enough parameters");
				}
				channel->changeLmode(Util::toInt(params[paramIdx++]));
			} else{
				channel->changeLmode(0);
			}
			response = ":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getIP() + " MODE " + canal + " " + (adding ? "+" : "-") + c + " " + (adding ? params[paramIdx - 1] : "");
			channel->broadcast(server, client, response, true);
			break;

		default:
			server.sendMessage(client, ":"+ server._name + " 472 " + client.getNickName() + \
									" " + c + " MODE :Is unknown mode char to me");
			break;
		}
	}
}
