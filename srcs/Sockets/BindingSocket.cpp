#include "BindingSocket.hpp"

namespace ft
{

BindingSocket::BindingSocket(int domain, int service, int protocol, unsigned int port, u_long interface) : Socket(domain, service, protocol, port, interface)
{	
	connect_to_network(this->get_sock(), this->get_address());
}

int BindingSocket::connect_to_network(int sock, struct sockaddr_in address)
{
	return bind(sock, (struct sockaddr *)& address, sizeof(address));
}

}
