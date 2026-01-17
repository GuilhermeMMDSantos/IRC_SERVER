#include "Server.hpp"

Server* globalServer = NULL;

void signalHandler(int signum)
{
	std::cout << "\n Received interruption signal (" << signum <<  ")" << std::endl;
	if (globalServer)
		globalServer->stop();
	exit(signum);
}

int main(int argc, char* argv[])
{

	std::string host = "127.0.0.1";
	int port = 8080;
	
	if (argc >= 2)
		port = std::atoi(argv[1]);
	if (argc >= 3)
		host = argv[2];
	
	try
	{
		Server server(host, port);
		globalServer = &server;
		
		signal(SIGINT, signalHandler);
		signal(SIGTERM, signalHandler);
		
		server.start();
	}
	catch(const std::exception& e)
	{
		std::cerr << "Fatal Error: " << e.what() << std::endl;
		return 1;
	}
	
	return 0;
}
