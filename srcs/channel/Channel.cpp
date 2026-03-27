#include "Channel.hpp"

Channel::Channel(const std::string& name)
: _name(name), _topic("")
{}

const std::string& Channel::getName() const{
	return _name;
}

const std::string& Channel::getTopic() const{
	return _topic;
}


void Channel::setTopic(const std::string& topic){
	_topic = topic;
}


void Channel::addMember(Client* client, bool isOperator){
	int fd = client->getFd();
	_members.insert(std::make_pair())
	_members[fd] = client;
	_operators[fd] = isOperator;
}
void Channel::removeMember(int fd){
	_members.erase(fd);
	_operators.erase(fd);
}

bool Channel::isMember(int fd) const{
	return _members.find(fd) != _members.end();
}

bool Channel::isOperator(int fd) const{
	std::map<int, bool>::const_iterator it = _operators.find(fd);
	if (it == _operators.end())
		return false;
	return it->second;
}

std::map<int, Client*>& Channel::getMembers(){
	return _members;
}

void	Channel::changeImode(bool b){
	_modes.updateImode(b);
}

void	Channel::changeTmode(bool b){
	_modes.updateTmode(b);
}
void	Channel::changeKmode(bool b, const std::string& key){
	_modes.updateKmode(b, key);
}
void	Channel::changeOmode(bool b){
	_modes.updateOmode(b);
}
void	Channel::changeLmode(bool b){
	_modes.updateLimit(b);
}

std::string	Channel::getActivesModes(const std::string& channel){
	std::string act = "";
	if (_modes.getImode()) act += "i";
	if (_modes.getTmode()) act += "t";
	if (_modes.getKmode()) act += "k";
	if (_modes.getOmode()) act += "o";
	if (_modes.getLimit() > 0) act += "l";
	return act;
}

void	Channel::removeOperator(const std::string& nick){
	Client* target = Util::getClientByNick(_members, nick);
	if (!target)return;

}
