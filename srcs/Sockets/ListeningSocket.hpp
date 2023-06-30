#ifndef __LISTENING_SOCKET__
#define __LISTENING_SOCKET__

#include "BindingSocket.hpp"

namespace ft
{

class ListeningSocket : public BindingSocket
{
	private:
		int _backlog;
	public:
		ListeningSocket(int domain, int service, int protocol, unsigned int port, u_long interface, int backlog);

		int start_listening();
};
}

#endif
