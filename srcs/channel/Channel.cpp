#include "Channel.hpp"

Channel::Channel(const std::string& name)
: _name(name), _topic(""), _password("")
{}

const std::string& Channel::getName() const{
    return _name;
}

const std::string& Channel::getTopic() const{
    return _topic;
}

bool Channel::hasPassword() const{
    return !_password.empty();
}

bool Channel::checkPassword(const std::string& pwd) const{
    return _password == pwd;
}


void Channel::setTopic(const std::string& topic){
    _topic = topic;
}


void Channel::addMember(Client* client, bool isOperator){
    int fd = client->getFd();
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

std::string     Channel::getMemberList() const{
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
