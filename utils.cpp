#include "utils.hpp"


bool hasJustNumberEcoma(std::string str){
	for (size_t i = 0; i < str.size(); i++){
		if (str[i] != '.' && !std::isdigit(str[i]))
			return false;
	}
	return true;
}

bool has3coma(std::string str){
	int count = 0;
	for (size_t i = 0; i < str.size(); i++){
		if (str[i] == '.')
			count++;
	}
	return (count == 3);
}

std::vector<unsigned int> parseIpv4(const std::string& str){
	int i = 0;
	std::stringstream ss(str);
	std::stringstream wordStream;
	std::string word;
	std::vector<unsigned int> values = std::vector<unsigned int>();

	if (str.size() == 0)
		throw std::runtime_error("ERROR: Empty ip string");
	if (!hasJustNumberEcoma(str) || !has3coma(str))
		throw std::runtime_error("ERROR: Sintax ip string");
	while (std::getline(ss,word,'.'))
	{
		if (word.size() == 0 || word.size() > 3)
			throw std::runtime_error("ERROR: Empty octeto in ip string");
		if (word.size() > 1 && word[0] == '0')
			throw std::runtime_error("ERROR: Leading zero in octeto in ip string");
		wordStream.str(word);
		values.push_back(0);
		wordStream >> values[i];
		if (values[i] > 255)
			throw std::runtime_error("ERROR: Octeto in ip string more than 255");
		wordStream.clear();
		i++;
	}

	if (i != 4)
		throw std::runtime_error("ERROR: Sintax ip string");
	return values;
}

in_addr_t stringToIpv4(const std::string& ipStr){
	try{
		std::vector<unsigned int>octetos = parseIpv4(ipStr);
		return htonl((octetos[0] << 24) | (octetos[1] << 16) | (octetos[2] << 8) | octetos[3]);
	}catch(std::exception& e){
		throw std::runtime_error(e.what());
	}
}

std::string ipv4ToString(const unsigned int ipv4)
{
	unsigned int aux1 = (1 << 8) - 1;
	std::stringstream ss;
	int deslocamento = 32;
	while (deslocamento > 0){
		deslocamento -=8;
		unsigned int aux2 = (ipv4 >> deslocamento) & aux1;
		ss << aux2;
		if (deslocamento != 0)
			ss << ".";
	}
	return  ss.str();
}
