#include "Server.hpp"

namespace ft
{

Server::Server(int domain, int service, int protocol, unsigned int port, u_long interface, int backlog)
{
	this->_socket = new ListeningSocket(domain, service, protocol, port, interface, backlog);
}

Server::~Server()
{
	delete this->_socket;
}

ListeningSocket *Server::get_socket() const
{
	return this->_socket;
}

}
