#pragma once

#include "BindingSocket.hpp"

namespace ft
{

class ListeningSocket : public BindingSocket
{
	private:
		int _backlog;
	public:
		ListeningSocket(int domain, int service, int protocol, unsigned int port, u_long interface, int backlog);
		virtual ~ListeningSocket();

		int start_listening();
};

}
