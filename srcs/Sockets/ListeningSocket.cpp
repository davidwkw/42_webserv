#include "ListeningSocket.hpp"

namespace ft
{

ListeningSocket::ListeningSocket(int domain, int service, int protocol, unsigned int port, u_long interface, int backlog) : BindingSocket(domain, service, protocol, port, interface)
{
	int status;

	this->_backlog = backlog;
	status = this->start_listening();
	test_connection(status);
}

int ListeningSocket::start_listening()
{
	return listen(this->get_sock(), _backlog);
}

}
