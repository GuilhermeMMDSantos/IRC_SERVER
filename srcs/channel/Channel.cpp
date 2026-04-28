#include "Channel.hpp"
#include "Server.hpp"

Channel::Channel(const std::string& name)
: _name(name), _topic(""), _userCount(0){}

const std::string& Channel::getName() const{
	return _name;
}

const std::string& Channel::getTopic() const{
	return _topic;
}

bool Channel::hasPassword() const{
	return _modes.getKmode();
}

bool Channel::checkPassword(const std::string& pwd) const{
	return _modes.getKey() == pwd;
}


void Channel::setTopic(const std::string& topic){
	_topic = topic;
}


void Channel::addMember(Client& client, bool isOperator){
	int fd = client.getFd();
	_members.insert(std::make_pair(fd, &client));
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

std::map<int, Client*>& Channel::getMembers(){
	return _members;
}

bool	Channel::isFirstMember() const{
	return _members.empty();
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

bool	Channel::hasTmode() const{
	return _modes.getTmode();
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
	Client* target = getClientByNick(nick);
	if (!target)return false;
	_operators[target->getFd()] = false;
	return true;
}

bool	Channel::addOperator(const std::string& nick){
	Client* target = getClientByNick(nick);
	if (!target) return false;
	_operators[target->getFd()] = true;
	return true;
}

std::string		Channel::getMemberList() const{
	std::string list;

	for (std::map<int, Client*>::const_iterator it = _members.begin(); it != _members.end(); ++it){
		if (!list.empty())
			list += " ";

		std::map<int, bool>::const_iterator opIt = _operators.find(it->first);
		if (opIt != _operators.end() && opIt->second)
			list += "@";
		list += it->second->getNickName();
	}
	return list;
}

void	Channel::broadcast(Server& server, Client& client, const std::string& msg, bool sendSelf){
	std::map<int, Client*>::iterator it = _members.begin();
	for (; it != _members.end(); it++){
		if (!sendSelf && it->first == client.getFd()) continue;
		if (it->second)
			server.sendMessage(*(it->second), msg);
	}
}

Client*	Channel::getClientByNick(const std::string& nick){
	for (std::map<int, Client*>::iterator it = _members.begin(); it != _members.end(); ++it){
		Client* c = it->second;
		if (c && Util::comp(c->getNickName(), nick))
			return c;
	}
	return NULL;
}

bool	Channel::isEmpty() const{
	return _members.empty();
}

bool	Channel::hasImode() const{
	return _modes.getImode();
}

int		Channel::hasLmode() const{
	return _modes.getLimit();
}



void	Channel::addToInviteList(const std::string& nick){
	_inviteList.push_back(nick);
}

bool	Channel::wasInvited(const std::string& nick) const{
	for(size_t i = 0; i < _inviteList.size(); i++){
		if (_inviteList[i] == nick) return true;
	}
	return false;
}

int	Channel::getUserCout() const{
	return _userCount;
}

void	Channel::removerFromInvite(const std::string& nick){
	_inviteList.erase(std::remove(_inviteList.begin(), _inviteList.end(), nick), _inviteList.end());
}
