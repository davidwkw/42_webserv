#include "ConnectingSocket.hpp"

namespace ft
{

ConnectingSocket::ConnectingSocket(int domain, int service, int protocol, unsigned int port, u_long interface, int buffer_size) : Socket(domain, service, protocol, port, interface, buffer_size)
{
	int status;

	status = this->connect_to_network(this->get_sock(), this->get_address());
	test_connection(status);
}

ConnectingSocket::~ConnectingSocket(){}

int ConnectingSocket::connect_to_network(int sock, struct sockaddr_in address)
{
	return connect(sock, (struct sockaddr *)&address, sizeof(address));
}

}
