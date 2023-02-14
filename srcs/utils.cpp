#include "webserv.hpp"
#include <cstring>
#include <cerrno>
#include <sstream>
#include <utility>
#include <string>

std::string ret_str_perror(const std::string &msg)
{
	std::stringstream ss;
	std::string str;

	ss << msg << ": " << std::strerror(errno) << std::endl;
	str = ss.str();
	return str;
}

std::string trim_ws_str(const std::string &str)
{
	std::stringstream temp(str);
	std::string word;
	std::string trimmmed_str;

	while (temp >> word)
		trimmmed_str += (trimmmed_str.empty() ? "" :  " " ) + word;

	return trimmmed_str;
}

std::string str_char_limit_span(const std::string &str, char open, char close){
	std::size_t start_index;
	std::size_t delimiter_index;
	std::size_t pair;
	std::string ret;
	std::string span_limiters;

	start_index = str.find_first_of(open);
	if (start_index == std::string::npos)
		return std::string();
	pair = 1;
	start_index += 1;
	delimiter_index = start_index;
	span_limiters += open;
	span_limiters += close;
	while ((pair > 0)
			&& (delimiter_index = str.find_first_of(span_limiters, delimiter_index)) != std::string::npos)
	{
		if (str[delimiter_index] == open){
			pair += 1;
		}
		else {
			pair -= 1;
		}
		++delimiter_index;
	}
	if (pair > 0)
    	return std::string(); 
	ret += str.substr(start_index, delimiter_index - start_index - 1);
	return ret;
}
