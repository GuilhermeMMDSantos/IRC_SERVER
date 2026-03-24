#include "Client.hpp"

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

bool	Client::extractMessage(std::string& line)
{
	size_t pos = _recvQ.find('\n');
	if (pos == std::string::npos || pos > 512) return false;

	line = _recvQ.substr(0, pos);
	_recvQ.erase(0, pos + 1);
	if (!line.empty() && line[line.size() - 1] == '\r')
		line.erase(line.size() -1);
	return true;
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


void	Client::updateNick(const std::string& nick){
	_identity.updateNickName(nick);
}

void	Client::updateUser(const std::string& nick){
	_identity.updateUserName(nick);
}
