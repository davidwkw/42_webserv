#include "Socket.hpp"

namespace ft
{

Socket::Socket(int domain, int service, int protocol, unsigned int port, u_long interface, int buffer_size)
{
	this->_address.sin_family = domain;
	this->_address.sin_port = htons(port);
	this->_address.sin_addr.s_addr = htonl(interface);
	this->_sock_fd = socket(domain, service, protocol);
	test_connection(_sock_fd);

	int enable_option = 1;

	if (setsockopt(this->_sock_fd, SOL_SOCKET, SO_REUSEADDR, &enable_option, sizeof(enable_option)) != 0)
	{
		throw std::runtime_error(ret_str_perror("Failed to set reuse addr socket option"));
	}

	int sock_buffer_size = buffer_size;

	if (setsockopt(this->_sock_fd, SOL_SOCKET, SO_RCVBUF, &sock_buffer_size, sizeof(sock_buffer_size)) != 0)
	{
		throw std::runtime_error(ret_str_perror("Failed to set receive buffer socket option"));
	}

	if (setsockopt(this->_sock_fd, SOL_SOCKET, SO_SNDBUF, &sock_buffer_size, sizeof(sock_buffer_size)) != 0)
	{
		throw std::runtime_error(ret_str_perror("Failed to set send buffer socket option"));
	}
}

Socket::~Socket()
{
	close(this->_sock_fd);
}

void Socket::test_connection(int item_to_test)
{
	if (item_to_test < 0)
	{
		throw std::runtime_error(ret_str_perror("Failed to connect"));
	}
}

struct sockaddr_in Socket::get_address() const
{
	return this->_address;
}

int Socket::get_sock() const
{
	return this->_sock_fd;
}

}
