#include "webserv.hpp"
#include <cstring>
#include <cerrno>

void ft_perror(const std::string &str)
{
	std::cerr << str << ": " << std::strerror(errno) << std::endl;
}
