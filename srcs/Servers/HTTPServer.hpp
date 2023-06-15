#ifndef __HTTP_SERVER__
#define __HTTP_SERVER__

#include <cstring>
#include "Server.hpp"
#include "../../includes/webserv.hpp"

namespace ft {

class HTTPServer : public Server 
{
	private:
		char _buffer[BUFFER_SIZE];
		int _accept_fd;

	public:
		HTTPServer(int port, int backlog);
		virtual ~HTTPServer();
		
		void accepter();
		void handler();
		void responder();
		void launch();
};

}

#endif
