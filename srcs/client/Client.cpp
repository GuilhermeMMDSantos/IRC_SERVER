#include "Client.hpp"

Client::Client(const ClientConnection& connection, std::string recvQ): _connection(connection), _recvQ(recvQ), _states(){};

void Client::updaterecvQ(std::string recvBuf)
{
	this->_recvQ += recvBuf;
}

bool	Client::extractMessage(std::string& line)
{
	size_t pos = _recvQ.find('\n');
	if (pos == std::string::npos) return false;

	line = _recvQ.substr(0, pos);
	_recvQ.erase(0, pos + 1);
	if (!line.empty() && line[line.size() - 1] == '\r')
		line.erase(line.size() -1);
	return true;
}
