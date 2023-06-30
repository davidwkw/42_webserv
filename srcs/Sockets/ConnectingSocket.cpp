#include "ConnectingSocket.hpp"

ft::ConnectingSocket::ConnectingSocket(int domain, int service, int protocol, unsigned int port, u_long interface) : Socket(domain, service, protocol, port, interface)
{
	int status;

	status = this->connect_to_network(this->get_sock(), this->get_address());
	test_connection(status);
}

int ft::ConnectingSocket::connect_to_network(int sock, struct sockaddr_in address)
{
	return connect(sock, (struct sockaddr *)&address, sizeof(address));
}
