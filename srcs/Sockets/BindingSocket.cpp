#include "BindingSocket.hpp"

namespace ft
{

BindingSocket::BindingSocket(int domain, int service, int protocol, unsigned int port, u_long interface, int buffer_size) : Socket(domain, service, protocol, port, interface, buffer_size)
{	
	int status;
	
	status = connect_to_network(this->get_sock(), this->get_address());
	test_connection(status);
}

BindingSocket::~BindingSocket(){}

int BindingSocket::connect_to_network(int sock, struct sockaddr_in address)
{
	return bind(sock, (struct sockaddr *)& address, sizeof(address));
}

}
