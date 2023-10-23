#include "../utils/utils.hpp"
#include "Socket.hpp"

namespace ft
{

Socket::Socket(int domain, int service, int protocol, unsigned int port, u_long interface)
{
	this->_address.sin_family = domain;
	this->_address.sin_port = htons(port);
	this->_address.sin_addr.s_addr = htonl(interface);
	this->_sock_fd = socket(domain, service, protocol);
	test_connection(_sock_fd);
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
