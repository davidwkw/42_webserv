#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#include <fstream>
#include <iostream>
#include <exception>
#include <vector>
#include <set>
#include "Server.hpp"
// #include <webserv.hpp>

class Config
{
	private:
		// probably replace std::string with ServerConfig class
		// std::vector<Server>	server_configs;

		// Server stuff
		// listen
		// server_name
		// error_page
		// client_max_body_size

		static const std::set<std::string>	directives; 
		std::string							_path;

		Config(void);
		void _init(const std::string &filename);

	public:
		~Config(void);
		Config(const Config &ref);
		Config &operator=(const Config &ref);
		Config(const std::string &filename);

		const std::string &path(void) const;

		class ConfigurationFailed : public std::exception {
			public:
				const char* what() const throw();
		};
		
		class ConfigurationFailed : public std::exception {
			public:
				const char* what() const throw();
		};
		
		class ConfigurationFailed : public std::exception {
			public:
				const char* what() const throw();
		};
};

#endif
