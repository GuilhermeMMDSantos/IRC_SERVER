#ifndef __CLIENTIDENTITY_HPP__
#define __CLIENTIDENTITY_HPP__

#include <string>

class ClientIdentity
{
	private:
		std::string	_username;
		std::string	_nickname;

	public:
		ClientIdentity();
		~ClientIdentity();


};

#endif
