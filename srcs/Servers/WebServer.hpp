#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/time.h>
#include <unistd.h>
#include <string>
#include <csignal>

#include "HTTPServer.hpp"
#include "../../includes/webserv.hpp"

namespace ft
{

class HTTPServer;

class WebServer
{
	public:
		WebServer(const WebserverConfig &config);
		WebServer(const std::string &config_path);
		~WebServer();

		void		run();

		std::string	get_config_string();

	private:
		WebserverConfig							_webserver_config;
		std::map<unsigned int, HTTPServer *>	_port_http_server_map;
		fd_set									_all_fds;
		fd_set 									_read_fds;
		fd_set									_write_fds;

		void	_initialise_socket_fd();
		void	_reinitialize_fd_sets();
		void	_append_listen_sockets_to_allfd(int &max_fd);
		void	_append_read_sockets_to_readfd(int &max_fd);
		void	_append_write_sockets_to_writefd(int &max_fd);
		void	_accept_incoming_connections();
		void	_perform_socket_io();
		bool	_have_clients();
		void	_eject_all_clients();

};

}
