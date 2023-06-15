#include "Server.hpp"

namespace ft
{

Server::Server(int domain, int service, int protocol, int port, u_long interface, int backlog)
{
	// prevent program from exiting when writing to a broken pipe
	// signal(SIGPIPE, SIG_IGN);
	this->_socket = new ListeningSocket(domain, service, protocol, port, interface, backlog);
}

Server::~Server()
{
	delete this->_socket;
}

ListeningSocket *Server::get_socket()
{
	return this->_socket;
}

}
