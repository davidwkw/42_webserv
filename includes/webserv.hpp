#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <dirent.h>
#include <stdexcept>
#include <fstream>
#include <cstdio>
#include <string>
#include <iostream>
#include <istream>
#include <map>
#include <cctype>
#include <algorithm>
#include <unistd.h>
#include <cstring>
#include <sys/time.h>
#include <ctime>
#include <string>

#include "colors.hpp"
#include "../srcs/Configurations/webserv-configurations.hpp"
#include "../srcs/Logger/Logger.hpp"
#include "../srcs/autoindex_helper/autoindex_helper.hpp"
#include "../srcs/Sockets/webserv-sockets.hpp"
#include "../srcs/utils/utils.hpp"
#include "../srcs/HTTP/Request.hpp"
#include "../srcs/HTTP/Response.hpp"
#include "../srcs/Servers/HTTPServer.hpp"
#include "../srcs/Servers/WebServer.hpp"
#include "../srcs/Client/Client.hpp"
#include "../srcs/Exception/HTTPException.hpp"
#include "../srcs/Configurations/CommonServerConfig.hpp"
#include "../srcs/CGI/CGI.hpp"

#define PORT 8080
#define HTTP_PROTOCOL 1.1
#define DEFAULT_CONFIG_PATH "configs/default.conf"
#define ERROR_PAGE_DIR "public/"
#define CRLF "\r\n"
#define BUFFER_SIZE 8192
#define TIMEOUT 1000*5
#define BACKLOG 10 // need to do some calculation of how many concurrent clients my server can support based on the number of virtual servers
#define MAX_CLIENTS 10
#define	DIRECTORY_TEMPLATE_IDENTIFIER "{DIRECTORY}"
#define DIRECTORY_CONTENT_TEMPLATE_IDENTIFIER "{DIRECTORY_CONTENTS}"
#define AUTOINDEX_CACHE_DIR "public/autoindex_cache"
#define AUTOINDEX_TEMPLATE_PATH "public/autoindex.html"
#define TIMEOUT_SECONDS 1
#define TIMEOUT_MICROSECONDS 0 
#define CGI_READ_BUFFER_SIZE 1024
#define CGI_TIMEOUT 30

#endif
