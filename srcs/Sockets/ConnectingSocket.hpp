#ifndef __CONNECTING_SOCKET__
#define __CONNECTING_SOCKET__

#include "Socket.hpp"

namespace ft
{

class ConnectingSocket : public Socket
{
	public:
		ConnectingSocket(int domain, int service, int protocol, unsigned int port, u_long interface);
		int connect_to_network(int sock, struct sockaddr_in address);
};

}

#endif
