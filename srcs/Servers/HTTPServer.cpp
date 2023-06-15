#include "HTTPServer.hpp"

namespace ft
{

HTTPServer::HTTPServer(int port, int backlog) : Server(AF_INET, SOCK_STREAM, 0, port, INADDR_ANY, backlog)
{
}

HTTPServer::~HTTPServer()
{
	
}

void HTTPServer::accepter()
{	
	int read_bytes = 0;
	std::string request;

	struct sockaddr_in address = this->_socket->get_address();
	int addrlen = sizeof(address);
	this->_accept_fd = accept(this->_socket->get_sock(), (struct sockaddr *)& address, (socklen_t *)&addrlen);
	if (this->_accept_fd < 0)
	{
		throw std::runtime_error(ret_str_error("Failure to accept incoming connection"));
	}

	read_bytes = recv(this->_accept_fd, this->_buffer, BUFFER_SIZE, MSG_NOSIGNAL);
	std::memset(this->_buffer, 0, BUFFER_SIZE);
}

void HTTPServer::handler()
{
	 
}

void HTTPServer::responder()
{
	char *msg = "Hello world\n";

	send(this->_accept_fd, msg, strlen(msg), MSG_NOSIGNAL);
	close(this->_accept_fd);
}

void HTTPServer::launch()
{
	while (true)
	{
		std::cout << "========== Waiting ==========" << std::endl;
		this->accepter();
		this->handler();
		this->responder();
		std::cout << "========== DONE ==========" << std::endl;
	}
}

}
