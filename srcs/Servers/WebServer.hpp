#ifndef __HTTP_SERVER_HPP__
#define __HTTP_SERVER_HPP__

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string>
#include "../../includes/webserv.hpp"

namespace ft
{

class WebServer{
	private:
		fd_set 					_read_fds;
		fd_set					_write_fds;
		struct addrinfo 		*_servinfo;
		WebserverConfig			_webserver_config;
		std::multimap<long, HTTPServer> _servers;

		void _start_servers();

	public:
		WebServer();
		WebServer(const WebserverConfig &config);
		~WebServer();
		WebServer(const WebServer &ref);
		WebServer &operator=(const WebServer &ref);

		void run() const;
};

}

#endif
