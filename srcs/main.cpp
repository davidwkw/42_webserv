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
	ft::Logger logger("webserver_log.txt");
	std::string conf_path;
	
	conf_path = (argc == 2) ? argv[1] : DEFAULT_CONFIG_PATH;
	std::cerr << "Launching webserv with: " << conf_path << std::endl;
	try
	{
		ft::WebServer server(conf_path);

		server.run();
	}
	catch(const std::runtime_error& run_error)
	{
		std::cerr << run_error.what() << std::endl;
		logger.error(run_error.what());
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
