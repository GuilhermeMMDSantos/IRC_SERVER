#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__

#include <string>
#include <map>
#include "Client.hpp"
#include "Mode.hpp"
#include "util/Util.hpp"

class Server;

class Channel{
	private:
		std::string				_name;
		std::string				_topic;
		Modes					_modes;
		std::map<int, Client>	_members;
		std::map<int, bool>		_operators;
		int						_userCount;

		public:
			Channel(const std::string& name);
			const std::string&		getName() const;
			const std::string&		getTopic() const;

			void					setTopic(const std::string& topic);

			bool					hasPassword() const;
			bool					checkPassword( const std::string& pwd) const;

			void	addMember( Client& client, bool isOperator = false);
			void	removeMember(int fd);
			bool	removeOperator(const std::string&);
			bool	addOperator(const std::string&);
			bool	isMember(int fd) const;
			bool	isOperator(int fd) const;
			bool	isFirstMember() const;

			void	changeImode(bool);
			void	changeTmode(bool);
			void	changeKmode(bool, const std::string&);
			void	changeOmode(bool);
			void	changeLmode(int);

			void	broadcast(Server&, Client&, const std::string&);

			std::string	getActivesModes();

			std::map<int, Client>&	getMembers();
			std::string				getMemberList() const;

};

#endif
