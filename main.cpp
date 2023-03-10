#include <sys/types.h>
#include <sys/socket.h>
// #include <netinet/in.h>
#include <arpa/inet.h>
#include "webserv.hpp"
#include "Config.hpp"
#include "Logger.hpp"

int	main(int argc, char **argv)
{
	std::string config_path;
	Logger logger;

	if (argc > 2)
	{
		std::cerr << "Usage: ./webserv [configuration file]" << std::endl;
		return EXIT_FAILURE;
	}

	config_path = (argc == 2) ? argv[1] : DEFAULT_CONFIG_PATH;
	
	Config config(config_path);
	// parse config
	// make config class?

	// make socket
	// bind socket to address
	// listen to requests
	// while 1
	// spawn new process whenever request is accepted
	// parse headers
	// search for file from url
	// format and reserve to client
	// kill process
	// repeat

	int server_fd;
	
	if (server_fd = socket(AF_INET, SOCK_STREAM, 0) < 0)
	{
		logger.error(ret_str_error("Failure to create socket"));
		return EXIT_FAILURE;
	}

	// Zero initializing address. https://en.cppreference.com/w/cpp/language/zero_initialization
	struct sockaddr_in address;
	
	memset(&address, 0, sizeof(struct sockaddr_in));

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(PORT);

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) 
	{ 
		perror("Binding failed"); 
		return EXIT_FAILURE; 
	}

	if (listen(server_fd, BACKLOG) < 0) 
	{ 
		perror("Failed to listen using server socket"); 
		exit(EXIT_FAILURE); 
	}

	int 		new_socket;
	socklen_t 	addrlen;
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
	{
		perror("Failed to accept incoming request");            
		exit(EXIT_FAILURE);        
	}

	close(new_socket);
	return EXIT_SUCCESS;
}
