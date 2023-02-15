#ifndef __SERVER_CONFIG_HPP__
#define __SERVER_CONFIG_HPP__

#include <set>
#include <map>
#include "Location.hpp"
#include "Config.hpp"
#include "webserv.hpp"

class ServerConfig{
	private:
		static const std::set<std::string>	_valid_directives;
		static std::set<std::string>		_fill_valid_directives();

		std::map<std::string, std::string>	_directives;
		std::map<std::string, Location> 	_locations;
		
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

		void _init_directives();
	public:
		ServerConfig();
		~ServerConfig();
		ServerConfig(const ServerConfig &ref);
		ServerConfig &operator=(const ServerConfig &ref);
		ServerConfig(const std::string &server_str);

		const std::vector<Location> &locations() const;
		void add_location(Location location);
		void add_server_name();
		const std::string &server_name() const;
};

#endif
