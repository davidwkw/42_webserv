#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../includes/webserv.hpp"

int	main(int argc, char **argv, char **envp)
{
	if (argc > 2)
	{
		std::cerr << "Usage: ./webserv [configuration file]" << std::endl;
		return EXIT_FAILURE;
	}
	
	ft::Logger			logger("webserver_log.txt");
	ft::WebserverConfig	config((argc == 2) ? argv[1] : DEFAULT_CONFIG_PATH);
	ft::WebServer		server(config, envp);

	try
	{
		server.run();
	}
	catch(const std::runtime_error& run_error)
	{
		logger.error(run_error.what());
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}
