#include "Socket.hpp"

namespace ft
{

Socket::Socket(int domain, int service, int protocol, unsigned int port, u_long interface)
{
	// define address structure
	this->_address.sin_family = domain;
	this->_address.sin_port = htons(port);
	this->_address.sin_addr.s_addr = htonl(interface);
	this->_sock_fd = socket(domain, service, protocol);
	// if(setsockopt(this->_sock_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )  
    // {  
    //     perror("setsockopt");  
    //     exit(EXIT_FAILURE);  
    // }  
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
