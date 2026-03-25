#ifndef __CLIENTIDENTITY_HPP__
#define __CLIENTIDENTITY_HPP__

#include <string>

class ClientIdentity
{
	private:
		std::string	_username;
		std::string	_nickname;

	public:
		const std::string&	getNickName() const;
		const std::string&	getUserName() const;
		void		updateNickName(const std::string&);
		void		updateUserName(const std::string&);
};

#endif
