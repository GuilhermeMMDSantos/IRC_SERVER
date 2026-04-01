#include "Message.hpp"

Message::Message(): _pos(0){

}

void	Message::skipSpace(const std::string& msg){
	while (_pos < msg.size() && msg[_pos] == ' ')
		++_pos;
}

std::string	Message::getCommand() const{
	return _command;
}

std::vector<std::string>	Message::getParams() const{
	return _params;
}

bool	Message::parsePrefix(const std::string& msg){
	_prefix.clear();
	if (msg.empty()) return false;
	if (msg[_pos] != ':' ) return true;
	++_pos;
	if (std::isspace(msg[_pos]) || _pos > msg.size()) return false;
	_pos = msg.find(' ');
	if (_pos == std::string::npos) return true;
	_prefix = msg.substr(1, _pos - 1);
	_pos++;
	return !_prefix.empty();
}

bool	Message::parseCommand(const std::string& msg){
	_command.clear();
	if (msg.empty() || _pos >= msg.size() || _pos == std::string::npos) return false;

	if (msg[_pos] == ':' ) return false;
	if (std::isalpha(msg[_pos])){
		while (_pos < msg.size() && std::isalpha(msg[_pos])){
			_command += msg[_pos];
			++_pos;
		}
	} else if (std::isdigit(msg[_pos]))
	{
		if (_pos + 2 >= msg.size() || !std::isdigit(msg[_pos + 1]) || !std::isdigit(msg[_pos + 2]))
			return false;
		for (size_t i = 1; i <= 3; i++){
			_command += msg[_pos]; ++_pos;
		}
	} else
		return false;
	if (_pos < msg.size() && msg[_pos] != ' ') return false;
	return !_command.empty();
}

bool	Message::parseTrailing(const std::string&msg, std::string& trail){
	trail.clear();
	while (_pos < msg.size()){
		if (msg[_pos] == ':') return false;
		if (msg[_pos] == '\0') break ;
		trail += msg[_pos];
		++_pos;
	}
	return true;
}

bool	Message::parseMiddle(const std::string& msg, std::string& middle){
	middle.clear();
	while (_pos < msg.size()){
		if (msg[_pos] == ' ') break;
		middle += msg[_pos];
		++_pos;
	}
	return !middle.empty();
}

bool Message::parseParams(const std::string& msg){
	skipSpace(msg);
	if (_pos >= msg.size()) return true;
	if (msg[_pos] == ':'){
		std::string	trail;
		++_pos;
		if (!parseTrailing(msg, trail)) return false;
		_params.push_back(trail);
		return true;
	} else {
		std::string	middle;
		if (!parseMiddle(msg, middle)) return false;
		_params.push_back(middle);
		return parseParams(msg);
	}
}

bool	Message::parse(const std::string& msg){
	_pos = 0;
	if (msg.empty()) return false;

	if(!parsePrefix(msg))	return (clear(), false);
	skipSpace(msg);
	if (!parseCommand(msg)) return (clear(), false);
	if (_pos < msg.size() && msg[_pos] == ' '){
		_params.clear();
		if (!parseParams(msg))
			return false;
	}
	return true;
}


void	Message::clear(){
	_prefix.clear();
	_command.clear();
	_params.clear();
}
