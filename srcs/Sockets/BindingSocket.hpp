#ifndef __BINDING_SOCKET__
#define __BINDING_SOCKET_

#include "Socket.hpp"

namespace ft
{

class BindingSocket : public Socket
{
	public:
		BindingSocket(int domain, int service, int protocol, int port, u_long interface);
		int connect_to_network(int sock, struct sockaddr_in address);
};
}

#endif
