#pragma once

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
		public:
			Socket(int domain, int service, int protocol, unsigned int port, u_long interface);
			virtual ~Socket();

			void		test_connection(int);
			virtual int connect_to_network(int sock, struct sockaddr_in address) = 0;

			struct sockaddr_in	get_address() const;
			int					get_sock() const;
		private:
			int		_sock_fd;
			struct sockaddr_in _address;
	};

}
