#include "HTTPServer.hpp"

namespace ft
{

HTTPServer::HTTPServer(unsigned int port, int backlog, unsigned int max_clients, unsigned int buffer_size, std::vector<ServerConfig> server_configs) 
: Server(AF_INET, SOCK_STREAM, 0, port, INADDR_ANY, backlog), _fd_to_client_map(), _fd_to_last_activity_map(), _server_configs(server_configs), _client_read_fds(), _client_write_fds(), _port(port), _max_clients(max_clients), _buffer_size(buffer_size){}

HTTPServer::~HTTPServer()
{
	this->clear();
}

void HTTPServer::accept_connection()
{	
	int					accept_fd;
	struct sockaddr_in	address = this->_socket->get_address();
	socklen_t			addrlen = sizeof(address);

	accept_fd = accept(this->_socket->get_sock(), (struct sockaddr *) &address, &addrlen);
	if (accept_fd < 0)
	{
		return;
	}
	try
	{
		this->_fd_to_client_map.insert(std::make_pair(accept_fd, new Client(accept_fd, this->_buffer_size)));
		this->_fd_to_last_activity_map.insert(std::make_pair(accept_fd, time(NULL)));
	}
	catch(const std::exception& e)
	{
		return;
	}
	insert_into_client_read_fds(accept_fd);
}

void HTTPServer::handle_request(const int &fd)
{
	try
	{
		this->_fd_to_last_activity_map.at(fd) = time(NULL);

		Client		&current_client = *(this->_fd_to_client_map.at(fd));
		std::size_t	bytes_read;

		bytes_read = current_client.read_to_buffer();
		if (bytes_read <= 0)
		{
			this->remove_fd(fd);
			close(fd);
			return;
		}

		if (current_client.get_client_state() == Client::READING_REQUEST_HEADERS)
		{
			current_client.handle_request_headers();
			if (current_client.get_client_state() == Client::REQUEST_HEADERS_CONSTRUCTED)
			{
				this->_assign_config_to_client(fd);
			}
		}
		
		if (current_client.get_client_state() == Client::REQUEST_HEADERS_CONSTRUCTED)
		{
			current_client.check_request_body();
		}
		
		if (current_client.get_client_state() == Client::READING_REQUEST_BODY)
		{
			std::cerr << "handling request body" << std::endl;
			current_client.handle_request_body();
		}
		
		if (current_client.get_client_state() == Client::VALIDATING_REQUEST
		|| current_client.get_client_state() == Client::PROCESSING_EXCEPTION)
		{
			insert_into_client_write_fds(fd);
			remove_from_client_read_fd(fd);
		}
	}
	catch (const std::out_of_range &e)
	{
		this->remove_fd(fd);
	}
}

void HTTPServer::handle_response(const int &fd)
{
	try
	{
		this->_fd_to_last_activity_map.at(fd) = time(NULL);
		Client &client = *(this->_fd_to_client_map.at(fd));

		client.handle_response();
		if (this->_fd_to_client_map.at(fd)->get_client_state() == Client::FINISHED_PROCESSING)
		{
			close(fd);
			this->remove_fd(fd);
		}
	}
	catch(const std::out_of_range& e)
	{
		this->remove_fd(fd);
	}
}

void	HTTPServer::timeout_idle_connections(double timeout)
{
	std::list<int>	timed_out_connections;

	for (std::map<int, time_t>::iterator it = this->_fd_to_last_activity_map.begin(); it != this->_fd_to_last_activity_map.end(); it++)
	{
		const int	&fd = it->first;
		time_t		&last_activity = it->second;

		if (std::difftime(time(NULL), last_activity) > timeout)
		{
			timed_out_connections.push_back(fd);
		}
	}

	for (std::list<int>::iterator it = timed_out_connections.begin(); it != timed_out_connections.end(); it++)
	{
		int	&fd = *it;
		
		close(fd);
		this->remove_fd(fd);
	}
}

void	HTTPServer::clear()
{
	std::for_each(this->_client_read_fds.begin(), this->_client_read_fds.end(), close_fd_helper);
	std::for_each(this->_client_write_fds.begin(), this->_client_write_fds.end(), close_fd_helper);
	this->_client_read_fds.clear();
	this->_client_write_fds.clear();

	for (std::map<int, Client*>::iterator it = this->_fd_to_client_map.begin(); it != this->_fd_to_client_map.end(); it++)
	{
		Client *current_client_ptr = it->second;

		delete current_client_ptr;
		current_client_ptr = NULL;
	}
	this->_fd_to_client_map.clear();
}

bool	HTTPServer::have_clients() const
{
	return !this->_fd_to_client_map.empty();
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
	Client	*client;

	try
	{
		client = this->_fd_to_client_map.at(fd);
		delete client;
	}
	catch (const std::out_of_range &e){}
	this->_fd_to_client_map.erase(fd);
	this->_fd_to_last_activity_map.erase(fd);
	remove_from_client_read_fd(fd);
	remove_from_client_write_fd(fd);
}

void HTTPServer::_assign_config_to_client(const int &fd)
{
	bool 		config_found = false;
	long 		server_config_index = 0;
	std::string origin_domain;

	try
	{
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
		this->_fd_to_client_map.at(fd)->set_server_config(new ServerConfig(this->_server_configs[server_config_index]));
	}
	catch (const std::out_of_range &e)
	{
		throw e;
	}
}

}
