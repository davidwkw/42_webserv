#ifndef __HTTP_SERVER_HPP__
#define __HTTP_SERVER_HPP__

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string>
#include "webserv.hpp"

namespace ft
{

class WebServer{
	private:
		int 			_sock_fd;
		struct addrinfo *_servinfo;
	public:
		WebServer();
		~WebServer();
		WebServer(const WebServer &ref);
		WebServer &operator=(const WebServer &ref);
		WebServer(const Config &config);

		void run() const;
};

}

// std::ostream& operator<<(std::ostream& os, const WebServer& obj)
// {
//     std::cout << obj.get_server_fd() << std::endl;
//     return os;
// }

#endif
