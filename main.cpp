#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../includes/webserv.hpp"

int	main(int argc, char **argv)
{
	if (argc > 2)
	{
		std::cerr << "Usage: ./webserv [configuration file]" << std::endl;
		return EXIT_FAILURE;
	}
	
	ft::WebserverConfig		config((argc == 2) ? argv[1] : DEFAULT_CONFIG_PATH);
	ft::WebServer	server(config);

	server.run();

	return EXIT_SUCCESS;
}
