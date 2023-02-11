#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <vector>
#include <set>
#include "Location.hpp"

class Server{
	private:
		static const std::set<std::string>	directives;

		std::vector<Location> locations;
		// Location directives
			// limit_except
			// root
			// autoindex
			// index
			// return
			// try_files
			// fastcgi_pass
			// fastcgi_params
			// upload_store?
			// location
	public:
		Server();
		~Server();
		Server(const Server &ref);
		Server &operator=(const Server &ref);
		Server(const std::string &server_str);

		const std::vector<Location> getLocations() const;

		void addLocation(Location location);
		void addServerName();
};

#endif
