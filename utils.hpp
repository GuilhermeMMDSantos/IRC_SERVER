#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <netinet/in.h>
#include <sstream>
#include "utils.hpp"
#include <climits>
#include <cctype>
#include <vector>

bool hasJustNumberEcoma(std::string str);
bool has3coma(std::string str);
std::vector<unsigned int> parseIpv4(const std::string& str);
in_addr_t stringToIpv4(const std::string& ipStr);
std::string ipv4ToString(const unsigned int ipv4);
#endif
