#include "Socket.hpp"

namespace ft
{

Socket::Socket(int domain, int service, int protocol, int port, u_long interface)
{
	// define address structure
	_address.sin_family = domain;
	_address.sin_port = htons(port);
	_address.sin_addr.s_addr = htonl(interface);
	_sock_fd = socket(domain, service, protocol);
	test_connection(_sock_fd);
}

Socket::Socket(const struct addrinfo &addr)
{
}

Socket::~Socket()
{
	if (close(this->_sock_fd) < 0)
		throw std::runtime_error(ret_str_error("Error closing socket"));
}

void Socket::test_connection(int item_to_test)
{
	if (item_to_test < 0)
	{
		throw std::runtime_error(ret_str_error("Failed to connect"));
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
