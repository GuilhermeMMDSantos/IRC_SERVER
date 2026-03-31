#include "Client.hpp"
#include <iostream>

Client::Client(const ClientConnection& connection, std::string recvQ): _connection(connection), _recvQ(recvQ), _states(){};

bool Client::updaterecvQ(std::string recvBuf)
{
	this->_recvQ += recvBuf;
	if (_recvQ.size() > 512 && _recvQ.find('\n') == std::string::npos){
		return false;
	}
	if (_recvQ.size() > 1024)
		return false;
	return true;
}

int	Client::extractMessage(std::string& line)
{
	size_t pos = _recvQ.find('\n');
	if (pos == std::string::npos){
		if (_recvQ.size() > 512){
			std::cerr << "Warning: Buffer overflow (> 512 bytes sem newline). Buffer cleared." << std::endl;
			_recvQ.clear();
			return -1;
		}
		return 0;
	}

	if (pos > 512){
		std::cerr << "Warning: Line too long (> 512 bytes), discarding: " << _recvQ.substr(0, pos) << std::endl;
		_recvQ.erase(0, pos + 1);
		return -2;
	}

	line = _recvQ.substr(0, pos);
	_recvQ.erase(0, pos + 1);
	if (!line.empty() && line[line.size() - 1] == '\r')
		line.erase(line.size() - 1);

	if (line.empty())
		return 0;

	return 1;
}

bool	Client::isRegisted() const{
	return _states.isRegisted;
}

bool	Client::hasPassword() const{
	return _states.hasPassWord;
}

bool	Client::hasNick() const{
	return _states.hasNickName;
}

bool	Client::hasUser() const{
	return _states.hasUserName;
}


int		Client::getFd() const{
	return _connection.getfd();
}
std::string	Client::getNickName() const{
	return _identity.getNickName();
}

void	Client::passwordConfirmed(){
	_states.hasPassWord = true;
}

void	Client::nickCreated(){
	_states.hasNickName = true;
}

void	Client::userCreated(){
	_states.hasUserName = true;
}


void	Client::updateNick(const std::string& nick){
	_identity.updateNickName(nick);
}

void	Client::updateUser(const std::string& nick){
	_identity.updateUserName(nick);
}

void	Client::updateRealName(const std::string& nick){
	_identity.updateRealName(nick);
}

void	Client::registreConfirmed(){
	_states.isRegisted = true;
}

std::string	Client::getUserName() const{
	return _identity.getUserName();
}
std::string	Client::getIP() const{
	return _connection.getIP();
}
