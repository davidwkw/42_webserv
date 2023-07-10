#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#include <fstream>
#include <iostream>
#include <exception>
#include <vector>
#include <set>
#include "Config.hpp"
#include "ServerConfig.hpp"
#include "../../includes/webserv.hpp"
#include <sstream>
#include <stdexcept>

namespace ft
{

class WebserverConfig : public Config
{
	private:
		std::string									_path;
		std::map<long, ServerConfig>				_servers;
		std::map<unsigned int, std::vector<long> >	_port_server_config_map;

		static const char					*all_directives_array[];
		static const char 					*normal_directives_array[];
		static const char 					*array_directives_array[];
		static const char 					*block_directives_array[];

		static const std::set<std::string> 	all_directives_set;
		static const std::set<std::string> 	normal_directives_set;
		static const std::set<std::string> 	array_directives_set;
		static const std::set<std::string> 	block_directives_set;

		bool 		_open_file(const std::string &filename, std::ifstream& file);
		void 		_cache_stream(std::ifstream& file, std::stringstream &cached_stream);
		std::string _parse_readable_lines(std::stringstream &cached_stream);
		void 		_parse_server_conf(const std::string &cached_string);

	public:
		WebserverConfig();
		~WebserverConfig();
		WebserverConfig(const WebserverConfig &ref);
		WebserverConfig &operator=(const WebserverConfig &ref);
		WebserverConfig(const std::string &filename);

		const std::string 							&path() const;
		const std::map<long, ServerConfig>			servers() const;
		std::map<unsigned int, std::vector<long> >	get_port_server_config_map() const;
};

}

#endif
