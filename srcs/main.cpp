#include <csignal>
#include <cstdlib>
#include "Server.hpp"

Server* globalServer = NULL;

void signalHandler(int signum){
	(void)signum;
	std::cout << "\nSignal received, shutting down..." << std::endl;
	if (globalServer)
		globalServer->stop();
}

bool isValidPort(const std::string& str){
	for (size_t i = 0; i < str.size(); ++i){
		if (str[i] < '0' || str[i] > '9')
			return false;
	}
	int port = std::atoi(str.c_str());
	return (port >= 1 && port <= 65535);
}

int main(int argc, char* argv[])
{

	if (argc != 3){
		std::cerr << "exec: ./ircserv <port> <password>" << std::endl;
		return 1;
	}

	if (!isValidPort(argv[1])){
		std::cerr << "Error: invalid port '" << argv[1] << "' (must be 1 - 65535)" << std::endl;
		return 1;
	}

	std::string password = argv[2];
	if (password.empty()){
		std::cout << "Error: password connot be empty" << std::endl;
		return 1;
	}

	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);
	signal(SIGQUIT, signalHandler);
	signal(SIGPIPE, SIG_IGN);

	try
	{
		Server server(argv[1], password);
		globalServer = &server;
		server.start();
	}
	catch(const std::exception& e)
	{
		std::cerr << "Fatal Error: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}
