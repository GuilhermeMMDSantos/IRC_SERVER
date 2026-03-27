#ifndef __CHANNELMANAGER_HPP__
#define __CHANNELMANAGER_HPP__

#include <map>
#include <string>
#include "Channel.hpp"

class ChannelManager{

    private:
        std::map<std::string, Channel> _channels;

        bool    isValidName(const std::string& name) const;

    public:

        Channel*    getOrCreate(const std::string& name);
        Channel*    getChannel(const std::string& name);

        void        removeChannel(const std::string& name);
        bool        channelExists(const std::string& name) const;
};

#endif
