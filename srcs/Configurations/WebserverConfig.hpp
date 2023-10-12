#pragma once

#include <fstream>
#include <iostream>
#include <exception>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>

#include "../../includes/macros.hpp"
#include "../utils/utils.hpp"
#include "Config.hpp"
#include "ServerConfig.hpp"

namespace ft
{

class ServerConfig;

class WebserverConfig : public Config
{
	public:
		WebserverConfig(const WebserverConfig &ref);
		WebserverConfig &operator=(const WebserverConfig &ref);
		WebserverConfig(const std::string &filename);
		~WebserverConfig();

		std::map<unsigned int, std::vector<long> >	get_port_server_config_index_map() const;
		const std::string 							&path() const;
		const std::map<long, ServerConfig>			servers() const;
		std::string									config_string() const;

	private:
		std::string									_path;
		std::string									_config_str;
		std::map<long, ServerConfig>				_servers;
		std::map<unsigned int, std::vector<long> >	_port_server_config_index_map;

		static const char							*all_directives_array[];
		static const char 							*normal_directives_array[];
		static const char 							*array_directives_array[];
		static const char 							*block_directives_array[];

		static const std::set<std::string>			all_directives_set;
		static const std::set<std::string>			normal_directives_set;
		static const std::set<std::string>			array_directives_set;
		static const std::set<std::string>			block_directives_set;

		bool 		_open_file(const std::string &filename, std::ifstream& file);
		void		_cache_stream(const std::string &filename, std::stringstream &cache_stream);
		std::string _parse_readable_lines(std::stringstream &cached_stream);
		void 		_parse_server_conf(const std::string &cached_string);

};

}
