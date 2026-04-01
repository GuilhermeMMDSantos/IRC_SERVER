#include "Mode.hpp"

Modes::Modes():_imode(false), _tmode(false), _kmode(false), \
			_limit(0), _key(""){}

void	Modes::setKey(const std::string& key){
	_key = key;
}

void	Modes::updateImode(bool b){
	_imode = b;
}
void	Modes::updateTmode(bool b){
	_tmode = b;
}
void	Modes::updateKmode(bool b, const std::string& key){
	_kmode = b;
	if (b) setKey(key);
	else _key.clear();
}
void	Modes::updateLimit(int b){
	_limit = b;
}

bool	Modes::getTmode() const{
	return _tmode;
}

bool	Modes::getImode() const{
	return _imode;
}

bool	Modes::getKmode() const{
	return _kmode;
}

int	Modes::getLimit() const{
	return _limit;
}

std::string	Modes::getKey() const{
	return _key;
}
std::string	Modes::getLimittoString() const{
	std::ostringstream oss;
	oss << _limit;
	return oss.str();
}
