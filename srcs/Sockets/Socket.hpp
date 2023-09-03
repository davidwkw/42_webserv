#ifndef __SOCKET_HPP__
#define __SOCKET_HPP__

#include <netinet/in.h>
#include <sys/types.h>
#include <fcntl.h>
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
			Socket(int domain, int service, int protocol, unsigned int port, u_long interface);
			Socket(const struct addrinfo &addr);
			~Socket();

			void		test_connection(int);
			virtual int connect_to_network(int sock, struct sockaddr_in address) = 0;

			struct sockaddr_in	get_address() const;
			int					get_sock() const;
	};
}

# endif
