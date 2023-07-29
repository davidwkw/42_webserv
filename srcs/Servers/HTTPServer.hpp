#ifndef __HTTP_SERVER__
#define __HTTP_SERVER__

#include <cstring>
#include <list>
#include "Server.hpp"
#include "../../includes/webserv.hpp"

namespace ft {

class HTTPServer : public Server 
{
	private:
		std::map<int, Client>			_fd_to_client_map;
		std::vector<ServerConfig>		_server_configs;
		std::list<int>					_client_read_fds;
		std::list<int>					_client_write_fds;
		unsigned int 					_port;
		unsigned int 					_max_clients;
		unsigned int 					_buffer_size;

		void _assign_config_to_client(const int &fd);

	public:
		HTTPServer(unsigned int port, int backlog, unsigned int max_clients, unsigned int buffer_size, std::vector<ServerConfig> server_configs);
		virtual ~HTTPServer();

		void				accept_connection();
		void				handle_request(const int &fd);
		void				handle_response(const int &fd);

		int					get_listen_socket_fd() const;
		std::list<int>		get_client_write_fds() const;
		std::list<int>		get_client_read_fds() const;
		unsigned int		get_port() const;

		void				insert_into_client_read_fds(const int fd);
		void				insert_into_client_write_fds(const int fd);

		void				remove_fd(int fd);
		void				remove_from_client_read_fd(const int fd);
		void				remove_from_client_write_fd(const int fd);
};

}

#endif
