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
		virtual void accepter() = 0;
		virtual void handler() = 0;
		virtual void responder() = 0;
	public:
		Server(int domain, int service, int protocol , int port, u_long interface, int backlog);
		virtual ~Server();
		virtual void launch() = 0;
		ListeningSocket *get_socket();
};

}

#endif
