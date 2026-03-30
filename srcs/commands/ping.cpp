#include "Executor.hpp"
#include "Server.hpp"

void Executor::ping(Server& server, Client& client, const Message& msg) {
	if (msg.getParams().empty()) {
		server.sendMessage(client, ":" + server._name + " 409 " + client.getNickName() + " :No origin specified");
		return;
	}

	std::string token = msg.getParams()[0];
	server.sendMessage(client, ":" + server._name + " PONG " + server._name + " :" + token);
}
