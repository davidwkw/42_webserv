#pragma once

#include <set>
#include <map>
#include <vector>
#include <sstream>
#include <string>
#include <cstdlib>
#include "../utils/utils.hpp"
#include "LocationConfig.hpp"
#include "Config.hpp"
#include "WebserverConfig.hpp"
#include "CommonServerConfig.hpp"

namespace ft
{

class ServerConfig : public CommonServerConfig
{
	public:
		ServerConfig();
		~ServerConfig();
		ServerConfig(const ServerConfig &ref);
		ServerConfig &operator=(const ServerConfig &ref);
		ServerConfig(std::map<std::string, std::string> directives, const std::string &server_str);

		std::map<std::string, LocationConfig>		locations() const;
		std::vector<unsigned int>					ports() const;
		std::vector<std::vector<std::string> >		listen() const;
		std::set<std::string> 						server_names() const;
		std::string 								client_body_temp_path() const;

	private:
		std::map<std::string, LocationConfig> 		_locations;

		static const char							*all_directives_array[];
		static const char 							*normal_directives_array[];
		static const char 							*array_directives_array[];
		static const char 							*block_directives_array[];

		static const std::set<std::string>			all_directives_set;
		static const std::set<std::string>			normal_directives_set;
		static const std::set<std::string>			array_directives_set;
		static const std::set<std::string>			block_directives_set;

		void 		_parse_location_conf(const std::string &cached_string);

};

}
