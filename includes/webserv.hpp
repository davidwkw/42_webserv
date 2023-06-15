#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <stdexcept>

#include "colors.hpp"
#include "WebServer.hpp"
#include "../srcs/Configurations/webserv-configurations.hpp"
#include "../srcs/Logger/Logger.hpp"
#include "../srcs/Sockets/webserv-sockets.hpp"
#include "../srcs/utils/utils.hpp"
#include "../srcs/HTTP/Request.hpp"
#include "../srcs/HTTP/Response.hpp"

#define BACKLOG 5
#define PORT 8080
#define HTTP_VERSION "HTTP 1.1"
#define DEFAULT_CONFIG_PATH "./configs/default.conf"
#define CRLF "\r\n"
#define BUFFER_SIZE 1024

#endif
