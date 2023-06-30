#ifndef __HTTP_SERVER__
#define __HTTP_SERVER__

#include <cstring>
#include "Server.hpp"
#include "../../includes/webserv.hpp"

namespace ft {

class HTTPServer : public Server 
{
	private:
		std::vector<ServerConfig>		_server_configs;
		std::vector<int>				_client_read_fds;
		std::vector<int>				_client_write_fds;
		std::map<int, Request>			_fd_to_request_map;
		std::map<int, Response>			_fd_to_response_map;
		unsigned int 					_port;
		unsigned int 					_max_clients;
		unsigned int 					_buffer_size;

	public:
		HTTPServer(unsigned int port, int backlog, unsigned int max_clients, unsigned int buffer_size, std::vector<ServerConfig> server_configs);
		virtual ~HTTPServer();

		void				accept_connection();
		int					process_request(const int &fd);

		int					get_listen_socket_fd() const;
		std::vector<int>	get_client_write_fds() const;
		std::vector<int>	get_client_read_fds() const;
		unsigned int		get_port() const;

		void				insert_into_client_read_fd(const int fd);
		void				insert_into_client_write_fd(const int fd);

		void				remove_fd(int fd);
		void				remove_from_client_read_fd(const int fd);
		void				remove_from_client_write_fd(const int fd);
};

}

#endif
