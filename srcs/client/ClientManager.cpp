#include <ClientManager.hpp>

void	ClientManager::addClient(const ClientConnection& connection, std::string recvQ){
	int fd = connection.getfd();
	_clients.insert(std::make_pair(fd, Client(connection, recvQ)));
}

void	ClientManager::removeClient(int fd){
	_clients.erase(fd);
}

Client*	ClientManager::getClient(int fd)
{
	std::map<int, Client>::iterator	it = _clients.find(fd);
	if (it != _clients.end()) return &it->second;
	return NULL;
}

std::map<int, Client>& ClientManager::getAllClient(){
	return _clients;
}

void	ClientManager::updaterecvQ(int fd, std::string recvBuf){
	std::map<int, Client>::iterator it = _clients.find(fd);
	if (it != _clients.end())
		it->second.updaterecvQ(recvBuf);
}

bool	ClientManager::extractMessage(int fd, std::string& line){
	std::map<int, Client>::iterator it = _clients.find(fd);
	if (it == _clients.end())
		return false;
	return it->second.extractMessage(line);
}

void	ClientManager::clear(){
	_clients.clear();
}
