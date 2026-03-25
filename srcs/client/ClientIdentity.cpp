#include "ClientIdentity.hpp"

const std::string&	ClientIdentity::getNickName() const{
	return _nickname;
}
const std::string&	ClientIdentity::getUserName() const{
	return _username;
}
void		ClientIdentity::updateNickName(const std::string& nick){
	_nickname.clear();
	_nickname = nick;
}
void		ClientIdentity::updateUserName(const std::string& user){
	_username.clear();
	_username = user;
}

void		ClientIdentity::updateRealName(const std::string& real){
	_realname.clear();
	_realname = real;
}


