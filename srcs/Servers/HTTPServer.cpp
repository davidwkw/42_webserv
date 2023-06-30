#include "HTTPServer.hpp"

namespace ft
{

#pragma region Constructors

HTTPServer::HTTPServer(unsigned int port, int backlog, unsigned int max_clients, unsigned int buffer_size, std::vector<ServerConfig> server_configs) 
: Server(AF_INET, SOCK_STREAM, 0, port, INADDR_ANY, backlog), _port(port), _max_clients(max_clients), _buffer_size(buffer_size), _client_read_fds(max_clients, 0), _client_write_fds(max_clients, 0), _fd_to_request_map(), _fd_to_response_map(), _server_configs(server_configs)
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

void HTTPServer::accept_connection()
{	
	int accept_fd = 0;

	struct sockaddr_in address = this->_socket->get_address();
	int addrlen = sizeof(address);
	accept_fd = accept(this->_socket->get_sock(), (struct sockaddr *)& address, (socklen_t *)&addrlen);
	if (accept_fd < 0)
	{
		throw std::runtime_error(ret_str_error("Failure to accept incoming connection"));
	}
	insert_into_client_read_fd(accept_fd);
	this->_fd_to_request_map.insert(std::make_pair(accept_fd, Request()));
}

int HTTPServer::process_request(const int &fd)
{
	char *	buffer = static_cast<char*>(calloc(this->_buffer_size, sizeof(char)));
	int		read_bytes = 0;

	read_bytes = recv(fd, buffer, BUFFER_SIZE - 1, MSG_NOSIGNAL);
	if (read_bytes <= 0)
	{
		free(buffer);
		return -1;
	}
	else
	{    
		this->_fd_to_request_map[fd].append_to_request(buffer);
		this->_fd_to_request_map[fd].process_request();
		if (this->_fd_to_request_map[fd].get_request_read_state() == Request::FINISHED)
		{
			remove_from_client_read_fd(fd);
			insert_into_client_write_fd(fd);
			this->_fd_to_response_map.insert(std::make_pair(fd, Response()));
		}
		free(buffer);
		return 0;
	}
}

// void HTTPServer::return_response(const int &fd)
// {

	// send(this->fd, this->_fd_to_response_map[fd].get_string(), this->_fd_to_response_map[fd].get_string().length(), MSG_NOSIGNAL);
	// for (std::vector<int>::iterator it = this->_client_write_fds.begin(); it != this->_client_write_fds.end(); it++)
	// {
	// 	if (*it == fd)
	// 	{
	// 		*it = 0;
	// 		this->_fd_to_response_map.erase(fd);
	// 		break;
	// 	}
	// }
	// close(fd);
// }

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
	this->_fd_to_request_map.erase(fd);
	remove_from_client_read_fd(fd);
	this->_fd_to_response_map.erase(fd);
	remove_from_client_write_fd(fd);
}

#pragma endregion Setters

}
