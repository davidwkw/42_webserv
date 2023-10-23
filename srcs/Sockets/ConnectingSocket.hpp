#pragma once

#include "Socket.hpp"

namespace ft
{

class ConnectingSocket : public Socket
{
	public:
		ConnectingSocket(int domain, int service, int protocol, unsigned int port, u_long interface, int buffer_size);
		int connect_to_network(int sock, struct sockaddr_in address);
		~ConnectingSocket();
};

}
