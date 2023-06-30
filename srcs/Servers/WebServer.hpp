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
		fd_set 								_read_fds;
		fd_set								_write_fds;
		WebserverConfig						_webserver_config;
		std::map<unsigned int, HTTPServer>	_port_http_server_map;
		int 								_max_fd;
		char								**_envp;

		void _initialise_socket_fd();
		void _reset_fd_sets();
		void _append_listen_sockets_to_readfd();
		void _append_read_sockets_to_readfd();
		void _append_write_sockets_to_writefd();
		void _accept_incoming_connections();
		void _perform_socket_io();

	public:
		// WebServer();
		WebServer(const WebserverConfig &config, char **envp);
		~WebServer();

		void run();
};

}

#endif
