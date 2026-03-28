#ifndef __Util_HPP__
#define __Util_HPP__

#include <map>
#include <string>
#include <algorithm>
#include "Client.hpp"


class Server;


class Util
{
	public:
		static bool	comp(const std::string& s1, const std::string& s2) {
			if (s1.size() != s2.size()) return false;
			for (size_t i = 0; i < s1.size(); i++){
				if (std::tolower(s1[i]) != std::tolower(s2[i]))
					return false;
			}
			return true;
		}

		template <typename T>
		static typename T::mapped_type* getClientByNick(T& o, const std::string& nick) {
			typename T::iterator it;

			for (it = o.begin(); it != o.end(); ++it) {
				if (comp((it->second).getNickName(), nick))
					return &it->second;
			}
			return NULL;
		}
};

#endif
