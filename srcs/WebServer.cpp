#include "WebServer.hpp"

WebServer::WebServer(){}

WebServer::~WebServer(){
	freeaddrinfo(this->_servinfo);
}

WebServer::WebServer(const WebServer &ref) : _sock_fd(ref._sock_fd){
	*this = ref;
}

WebServer &WebServer::operator=(const WebServer &ref){
	if (this != &ref)
	{

	}
	return *this;
}

WebServer::WebServer(const Config &config){

    for (std::map<long, ServerConfig>::const_iterator cit = config.servers().begin(); cit != config.servers().end(); ++cit)
    {
        
    }
}
// WebServer::WebServer(const char *node, const char *port){
// 	int 			status;
// 	struct addrinfo	hints;

// 	memset(&hints, 0, sizeof(hints));
// 	hints.ai_family = AF_UNSPEC;
// 	hints.ai_socktype = SOCK_STREAM;
// 	hints.ai_flags = AI_PASSIVE; 
	
// 	if ((status = getaddrinfo(node, port, &hints, &this->_servinfo)) != 0){
// 		throw std::runtime_error("[WebServer] Unable to set up addrinfo: " + std::string(gai_strerror(status)));
// 	}

// }

void WebServer::run() const{

	struct addrinfo	*p;

	// for (p = this->_servinfo; p != NULL; p = p->ai_next){
    //     if ((this->_sock_fd = socket(p->ai_family, p->ai_socktype,
    //             p->ai_protocol)) == -1) {
    //         perror("server: socket");
    //         continue;
    //     }

    //     if (setsockopt(this->_sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes,
    //             sizeof(int)) == -1) {
    //         perror("setsockopt");
    //         exit(1);
    //     }

    //     if (bind(this->_sock_fd, p->ai_addr, p->ai_addrlen) == -1) {
    //         close(this->_sock_fd);
    //         perror("server: bind");
    //         continue;
    //     }

    //     break;
	// }
}
