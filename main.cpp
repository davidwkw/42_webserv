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

	ft::Logger logger("webserver_log.txt");
	
	try
	{
		ft::WebserverConfig	config((argc == 2) ? argv[1] : DEFAULT_CONFIG_PATH);

		ft::cache_autoindex(config.servers(), AUTOINDEX_TEMPLATE_PATH, AUTOINDEX_CACHE_DIR);

		ft::WebServer server(config, envp); // TODO kinda fugly, refactor this later

		server.run();
	}
	catch(const std::runtime_error& run_error)
	{
		logger.error(run_error.what());
		ft::clear_auto_index_cache(AUTOINDEX_CACHE_DIR);
		return EXIT_FAILURE;
	}
	ft::clear_auto_index_cache(AUTOINDEX_CACHE_DIR);
	return EXIT_SUCCESS;
}
