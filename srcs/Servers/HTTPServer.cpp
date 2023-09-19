#include "HTTPServer.hpp"

namespace ft
{

HTTPServer::HTTPServer(unsigned int port, int backlog, unsigned int max_clients, unsigned int buffer_size, std::vector<ServerConfig> server_configs) 
: Server(AF_INET, SOCK_STREAM, 0, port, INADDR_ANY, backlog), _fd_to_client_map(), _server_configs(server_configs), _client_read_fds(), _client_write_fds(), _port(port), _max_clients(max_clients), _buffer_size(buffer_size)
{}

HTTPServer::~HTTPServer()
{
	std::for_each(this->_client_read_fds.begin(), this->_client_read_fds.end(), close_fd_helper);
	std::for_each(this->_client_write_fds.begin(), this->_client_write_fds.end(), close_fd_helper);
	this->_client_read_fds.clear();
	this->_client_write_fds.clear();

	for (std::map<int, Client*>::iterator it = this->_fd_to_client_map.begin(); it != this->_fd_to_client_map.end(); it++)
	{
		delete it->second;
	}
	this->_fd_to_client_map.clear();
}

void HTTPServer::_assign_config_to_client(const int &fd)
{
	bool 		config_found = false;
	long 		server_config_index = 0;
	std::string origin_domain;
	
	origin_domain = this->_fd_to_client_map.at(fd)->get_request().get_header("Host");

	// Go through all Server configs of this server and search for a config with a matching domain; if unable to return first server.
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
	{
		server_config_index = 0;
	}
	this->_fd_to_client_map.at(fd)->set_server_config(&this->_server_configs[server_config_index]);
}

void HTTPServer::accept_connection()
{	
	int accept_fd = 0;

	struct sockaddr_in address = this->_socket->get_address();
	socklen_t addrlen = sizeof(address);
	accept_fd = accept(this->_socket->get_sock(), (struct sockaddr *)& address, &addrlen);
	if (accept_fd < 0)
	{
		return;
	}
	try
	{
		this->_fd_to_client_map.insert(std::make_pair(accept_fd, new Client(accept_fd, this->_buffer_size)));
		insert_into_client_read_fds(accept_fd);
	}
	catch(const std::exception& e)
	{
		return;
	}
}

void HTTPServer::handle_request(const int &fd)
{
	if (this->_fd_to_client_map.at(fd)->handle_request() <= 0)
	{
		this->remove_fd(fd);
		close(fd);
		return;
	}
	if (this->_fd_to_client_map.at(fd)->get_process_state() == Client::VALIDATING_REQUEST)
	{
		insert_into_client_write_fds(fd);
		remove_from_client_read_fd(fd);
		this->_assign_config_to_client(fd);
	}
}

void HTTPServer::handle_response(const int &fd)
{
	this->_fd_to_client_map.at(fd)->handle_response();
	if (this->_fd_to_client_map.at(fd)->get_process_state() == Client::FINISHED_PROCESSING)
	{
		this->_fd_to_client_map.erase(fd);
		this->remove_from_client_write_fd(fd);
		close(fd);
	}
}

int HTTPServer::get_listen_socket_fd() const
{
	return this->_socket->get_sock();
}

std::list<int> HTTPServer::get_client_read_fds() const
{
	return this->_client_read_fds;
}

std::list<int> HTTPServer::get_client_write_fds() const
{
	return this->_client_write_fds;
}

unsigned int HTTPServer::get_port() const
{
	return this->_port;
}

void HTTPServer::insert_into_client_read_fds(const int fd)
{
	this->_client_read_fds.push_back(fd);
}

void HTTPServer::insert_into_client_write_fds(const int fd)
{
	this->_client_write_fds.push_back(fd);
}

void HTTPServer::remove_from_client_read_fd(const int fd)
{
	this->_client_read_fds.remove(fd);
}

void HTTPServer::remove_from_client_write_fd(const int fd)
{
	this->_client_write_fds.remove(fd);
}

void HTTPServer::remove_fd(int fd)
{
	this->_fd_to_client_map.erase(fd);
	remove_from_client_read_fd(fd);
	remove_from_client_write_fd(fd);
}

}
