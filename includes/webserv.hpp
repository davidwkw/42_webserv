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
std::string trim_ws_str(const std::string &str);
std::string str_char_limit_span(const std::string &str, char open, char close);
std::vector<std::string> tokenise_str(const std::string & str, char c);

#endif
