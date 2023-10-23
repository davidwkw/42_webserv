#include "ListeningSocket.hpp"

namespace ft
{

ListeningSocket::ListeningSocket(int domain, int service, int protocol, unsigned int port, u_long interface, int backlog, int buffer_size) : BindingSocket(domain, service, protocol, port, interface, buffer_size)
{
	int status;

	this->_backlog = backlog;
	status = this->start_listening();
	test_connection(status);
}

ListeningSocket::~ListeningSocket(){}

int ListeningSocket::start_listening()
{
	return listen(this->get_sock(), _backlog);
}

}
