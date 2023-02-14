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
