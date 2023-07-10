#include "HTTPServer.hpp"

namespace ft
{

#pragma region Constructors

HTTPServer::HTTPServer(unsigned int port, int backlog, unsigned int max_clients, unsigned int buffer_size, std::vector<ServerConfig> server_configs) 
: Server(AF_INET, SOCK_STREAM, 0, port, INADDR_ANY, backlog), _port(port), _max_clients(max_clients), _buffer_size(buffer_size), _client_read_fds(max_clients, 0), _client_write_fds(max_clients, 0), _server_configs(server_configs)
{}

HTTPServer::~HTTPServer()
{
	for (std::vector<int>::iterator it = this->_client_read_fds.begin(); it != this->_client_read_fds.end(); it++)
	{
		if (*it != 0)
		{
			close(*it);
		}
	}
	for (std::vector<int>::iterator it = this->_client_write_fds.begin(); it != this->_client_write_fds.end(); it++)
	{
		if (*it != 0)
		{
			close(*it);
		}
	}
}

#pragma endregion Constructors

#pragma region ClassUtility

void HTTPServer::_assign_config_to_client(const int &fd)
{
	bool config_found = false;
	long server_config_index = 0;
	std::string origin_domain = this->_fd_to_client_map[fd].get_request().get_header("Host");

	for (std::vector<ServerConfig>::iterator it = this->_server_configs.begin(); it != this->_server_configs.end(); it++)
	{
		if (it->server_names().find(origin_domain) != it->server_names().end())
		{
			config_found = true;
			break;
		}
		server_config_index++;
	}

	if (!config_found)
		server_config_index = 0;
	
	this->_fd_to_client_map[fd].set_server_config(this->_server_configs[server_config_index]);
}

#pragma endregion ClassUtility

void HTTPServer::accept_connection()
{	
	int accept_fd = 0;

	struct sockaddr_in address = this->_socket->get_address();
	int addrlen = sizeof(address);
	accept_fd = accept(this->_socket->get_sock(), (struct sockaddr *)& address, (socklen_t *)&addrlen);
	if (accept_fd < 0)
	{
		// need to somehow handle this, this will kill the server immediately
		throw std::runtime_error(ret_str_error("Failure to accept incoming connection"));
	}
	insert_into_client_read_fd(accept_fd);
	this->_fd_to_client_map.insert(std::make_pair(accept_fd, Client(accept_fd, this->_buffer_size)));
}

void HTTPServer::handle_request(const int &fd)
{
	if (this->_fd_to_client_map[fd].handle_request() <= 0)
	{
		this->remove_fd(fd);
		close(fd);
		return;
	}
	if (this->_fd_to_client_map[fd].get_process_state() == Client::PROCESSING_RESPONSE)
	{
		insert_into_client_write_fd(fd);
		remove_from_client_read_fd(fd);
	}
}

void HTTPServer::handle_response(const int &fd)
{
	this->_assign_config_to_client(fd);
	this->_fd_to_client_map[fd].handle_response();
	if (this->_fd_to_client_map[fd].get_process_state() == Client::FINISHED_PROCESSING)
	{
		this->_fd_to_client_map.erase(fd);
		this->remove_from_client_write_fd(fd);
		close(fd);
	}
}

#pragma region Getters

int HTTPServer::get_listen_socket_fd() const
{
	return this->_socket->get_sock();
}

std::vector<int> HTTPServer::get_client_read_fds() const
{
	return this->_client_read_fds;
}

std::vector<int> HTTPServer::get_client_write_fds() const
{
	return this->_client_write_fds;
}

unsigned int HTTPServer::get_port() const
{
	return this->_port;
}

#pragma endregion Getters

#pragma region Setters

void HTTPServer::insert_into_client_read_fd(const int fd)
{
	for (std::vector<int>::iterator it = this->_client_read_fds.begin(); it != this->_client_read_fds.end(); it++)
	{
		if (*it == 0)
		{
			*it = fd;
			break;
		}
	}
}

void HTTPServer::insert_into_client_write_fd(const int fd)
{
	for (std::vector<int>::iterator it = this->_client_write_fds.begin(); it != this->_client_write_fds.end(); it++)
	{
		if (*it == 0)
		{
			*it = fd;
			break;
		}
	}
}

void HTTPServer::remove_from_client_read_fd(const int fd)
{
	for (std::vector<int>::iterator it = this->_client_read_fds.begin(); it != this->_client_read_fds.end(); it++)
	{
		if (*it == fd)
		{
			*it = 0;
			break;
		}
	}
}

void HTTPServer::remove_from_client_write_fd(const int fd)
{
	for (std::vector<int>::iterator it = this->_client_write_fds.begin(); it != this->_client_write_fds.end(); it++)
	{
		if (*it == fd)
		{
			*it = 0;
			break;
		}
	}
}

void HTTPServer::remove_fd(int fd)
{
	this->_fd_to_client_map.erase(fd);
	remove_from_client_read_fd(fd);
	remove_from_client_write_fd(fd);
}

#pragma endregion Setters

}
