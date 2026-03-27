#include "ChannelManager.hpp"
#include <cctype>

bool ChannelManager::isValidName(const std::string& name) const{
    if (name.empty() || name.size() < 2)
        return false;
    if (name[0] != '#' && name[0] != '&')
        return false;
    if (name.size() > 200)
        return false;
    for (size_t i = 1; i < name.size(); ++i){
        char c = name[i];
        if (c == ' ' || c == ',' || c == '\x07')
            return false;
    }
    return true;
}


Channel* ChannelManager::getOrCreate(const std::string& name){

    if (!isValidName(name))
        return NULL;

    if (_channels.find(name) == _channels.end())
        _channels.insert(std::make_pair(name, Channel(name)));

    return &_channels.at(name);
}

Channel* ChannelManager::getChannel(const std::string& name){

    std::map<std::string, Channel>::iterator it = _channels.find(name);
    if (it == _channels.end())
        return NULL;
    return &it->second;
}


void ChannelManager::removeChannel(const std::string& name){
    _channels.erase(name);
}

bool ChannelManager::channelExists(const std::string& name) const{
    return _channels.find(name) != _channels.end();
}
