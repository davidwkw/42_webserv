#include "webserv.hpp"
#include <cstring>
#include <cerrno>
#include <sstream>

std::string ret_str_perror(const std::string &msg)
{
	std::stringstream ss;
	std::string str;

	ss << msg << ": " << std::strerror(errno);
	str = ss.str();
	return str;
}
