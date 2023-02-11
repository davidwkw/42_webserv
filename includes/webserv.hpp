#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <string>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <exception>
#include <fstream>
#include <ios>

#define BACKLOG 5
#define PORT 8080
#define DEFAULT_CONFIG_PATH "./configs/default.conf"

std::string ret_str_error(const std::string &msg);

#endif
