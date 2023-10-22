#pragma once

#include <limits.h>

#define WHITESPACE_CHARACTERS " \t\n\r\f\v"
#define PORT 8080
#define HTTP_PROTOCOL 1.1
#define DEFAULT_CONFIG_PATH "configs/default.conf"
#define ERROR_PAGE_DIR "public/error_pages/"
#define DEFAULT_ERROR_PAGE_DIR "default/"
#define CRLF "\r\n"
#define BUFFER_SIZE 8192
#define TIMEOUT 1000*5
#define BACKLOG 10
#define MAX_CLIENTS 10
#define	DIRECTORY_TEMPLATE_IDENTIFIER "{DIRECTORY}"
#define DIRECTORY_CONTENT_TEMPLATE_IDENTIFIER "{DIRECTORY_CONTENTS}"
#define AUTOINDEX_CACHE_DIR "public/autoindex_cache"
#define AUTOINDEX_TEMPLATE_PATH "public/autoindex_template.html"
#define TIMEOUT_SECONDS 1
#define TIMEOUT_MICROSECONDS 0 
#define CGI_READ_BUFFER_SIZE 1024
#define CGI_TIMEOUT 5.0
#define CGI_GATEWAY_INTERFACE_VERSION 1.1
#define SERVER_NAME "ft_webserv"
#define SOFTWARE_VERSION 1.0
#define REQUEST_TIMEOUT 2.0
