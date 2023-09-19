#pragma once

#include "Socket.hpp"

namespace ft
{

class BindingSocket : public Socket
{
	public:
		BindingSocket(int domain, int service, int protocol, unsigned int port, u_long interface);
		int connect_to_network(int sock, struct sockaddr_in address);
		virtual ~BindingSocket();
};

}
