#ifndef __SOCKET_HPP__
#define __SOCKET_HPP__

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdexcept>
#include "../utils/utils.hpp"

namespace ft
{
	class Socket
	{
		private:
			int		_sock_fd;
			struct sockaddr_in _address;
		public:
			Socket(int domain, int service, int protocol, int port, u_long interface);
			Socket(const struct addrinfo &addr);
			~Socket();
			virtual int connect_to_network(int sock, struct sockaddr_in address) = 0;
			void test_connection(int);

			struct sockaddr_in get_address() const;
			int get_sock() const;

			void set_connection(int connection);
	};
}

# endif
