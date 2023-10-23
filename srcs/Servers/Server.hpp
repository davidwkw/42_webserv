#pragma once

#include <signal.h>

#include "../Sockets/webserv-sockets.hpp"

namespace ft
{

class Server
{
	public:
		Server(int domain, int service, int protocol , unsigned int port, u_long interface, int backlog, int buffer_size);
		virtual ~Server();
	
		ListeningSocket *get_socket() const;

	protected:
		ListeningSocket *_socket;
		
		virtual void accept_connection() = 0;

};

}
