#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__

#include <string>
#include <map>
#include "Client.hpp"

class Server;

class Channel{

    private:
        std::string             _name;
        std::string             _topic;
        std::map<int, Client*>  _members;
        std::map<int, bool>     _operators;

        public:
            Channel(const std::string& name);
            const std::string&      getName() const;
            const std::string&      getTopic() const;


            void    setTopic(const std::string& topic);

            void    addMember( Client* client, bool isOperator = false);
            void    removeMember(int fd);
            bool    isMember(int fd) const;
            bool    isOperator(int fd) const;

            std::map<int, Client*>& getMembers();
            void brodcast(Server&, Client&, const std::string&);
          //  std::string             getMemberList() const;

};

#endif