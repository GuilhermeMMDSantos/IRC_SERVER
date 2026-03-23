#ifndef __MESSAGE_HPP__
#define __MESSAGE_HPP__

#include <iostream>
#include <string>
#include <vector>
#include <cctype>

class Message
{
	private:
		std::string					_prefix;
		std::string					_command;
		std::vector<std::string>	_params;
		size_t						_pos;

		void	skipSpace(const std::string& msg);
		bool	parsePrefix(const std::string& msg);
		bool	parseCommand(const std::string& msg);
		bool	parseTrailing(const std::string&msg, std::string& trail);
		bool	parseMiddle(const std::string& msg, std::string& middle);
		bool	parseParams(const std::string& msg);
		// bool	parseTrailing();
	public:
		Message();

		std::string					getPrefix() const;
		std::string					getCommand() const;
		std::vector<std::string>	getParams() const;

		bool	parse(const std::string& msg);
		void	clear();
};

#endif
