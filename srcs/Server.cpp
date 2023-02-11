#include "Server.hpp"

const std::set<std::string> Server::directives = {
												limit_except
												root
												autoindex
												index
												return
												try_files
												fastcgi_pass
												fastcgi_params
												upload_store?								
												};
