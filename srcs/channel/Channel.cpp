#include "Channel.hpp"

Channel::Channel(const std::string& name)
: _name(name), _topic(""), _userCount(0)
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


void Channel::addMember(Client& client, bool isOperator){
	int fd = client.getFd();
	_members.insert(std::make_pair(fd, client));
	_operators.insert(std::make_pair(fd, isOperator));
	++_userCount;
}
void Channel::removeMember(int fd){
	_members.erase(fd);
	_operators.erase(fd);
	--_userCount;
	if (_userCount < 0)
		_userCount = 0;
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

std::map<int, Client>& Channel::getMembers(){
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

void	Channel::changeLmode(int b){
	_modes.updateLimit(b);
}

std::string	Channel::getActivesModes(){
	std::string act = "";
	std::vector<std::string> params;

	if (_modes.getImode()) act += "i";
	if (_modes.getTmode()) act += "t";
	if (_modes.getKmode()){
		act += "k";
		params.push_back(_modes.getKey());
	}
	if (_modes.getLimit() > 0){
		act += "l";
		params.push_back(_modes.getLimittoString());
	}
	for (size_t i = 0; i < params.size(); ++i) {
		act += " " + params[i];
	}
	return act;
}

bool	Channel::removeOperator(const std::string& nick){
	Client* target = Util::getObjectByString(_members, nick);
	if (!target)return false;
	_operators[target->getFd()] = false;
	return true;
}

bool	Channel::addOperator(const std::string& nick){
	Client* target = Util::getObjectByString(_members, nick);
	if (!target)return false;
	_operators[target->getFd()] = true;
	return true;
}
