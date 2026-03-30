#include "Executor.hpp"
#include "Server.hpp"

void	Executor::cap(Server& server, Client& client, const Message& msg){
	std::vector<std::string> params  = msg.getParams();
	if (params.empty()) return;
	if (params[0] == "END") return;
	server.sendMessage(client, ":"+ server._name + " CAP * LS :");
}
