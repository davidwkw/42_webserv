#include "webserv.hpp"
#include <cstring>
#include <cerrno>
#include <sstream>
#include <utility>
#include <string>
#include <vector>
#include <cstring>

std::string ret_str_perror(const std::string &msg)
{
	std::stringstream ss;
	std::string str;

	ss << msg << ": " << std::strerror(errno) << std::endl;
	str = ss.str();
	return str;
}

std::string trim_str(const std::string &str, char *chars)
{
	std::size_t start_index;
	std::size_t end_index;

	start_index = str.find_first_not_of(chars);
	end_index = str.find_last_not_of(chars);
	
	return str.substr(start_index, end_index - start_index + 1);
}

std::vector<std::string> tokenise_str(const std::string & str, char c = ' ')
{
	std::string temp;
	std::vector<std::string> ret_vector;
	std::stringstream ss(str);

	while (std::getline(ss, temp, c))
		ret_vector.push_back(trim_str(temp, " \n\t"));
	return ret_vector;
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
