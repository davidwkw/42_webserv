#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <signal.h>
#include "../Sockets/webserv-sockets.hpp"

namespace ft
{

class Server
{
	protected:
		ListeningSocket *_socket;
		virtual void accept_connection() = 0;
		// virtual void responder() = 0;

	public:
		Server(int domain, int service, int protocol , unsigned int port, u_long interface, int backlog);
		virtual ~Server();
	
		ListeningSocket *get_socket() const;
};

}

#endif
