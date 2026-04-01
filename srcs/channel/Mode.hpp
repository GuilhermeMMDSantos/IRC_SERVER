#ifndef __MODES_HPP__
#define __MODES_HPP__

#include <string>
#include <sstream>
#include <map>
#include "Client.hpp"
#include "Message.hpp"

class Modes{

	private:
		bool		_imode;
		bool		_tmode;
		bool		_kmode;
		int			_limit;
		std::string	_key;

		void	setKey(const std::string&);
	public:
		Modes();

		void	updateImode(bool);
		void	updateTmode(bool);
		void	updateKmode(bool, const std::string&);
		void	updateLimit(int);

		std::string	getKey() const;
		std::string	getLimittoString() const;
		bool	getTmode() const;
		bool	getImode() const;
		bool	getKmode() const;
		int		getLimit() const;

};

#endif
