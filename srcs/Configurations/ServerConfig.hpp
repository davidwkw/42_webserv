#ifndef __SERVER_CONFIG_HPP__
#define __SERVER_CONFIG_HPP__

#include <set>
#include <map>
#include <sstream>
#include "LocationConfig.hpp"
#include "Config.hpp"
#include "WebserverConfig.hpp"
#include "../../includes/webserv.hpp"

namespace ft
{

class ServerConfig : public Config
{
	private:
		std::map<std::string, LocationConfig> 	_locations;

		static const char					*all_directives_array[];
		static const char 					*normal_directives_array[];
		static const char 					*array_directives_array[];
		static const char 					*block_directives_array[];

		static const std::set<std::string> 	all_directives_set;
		static const std::set<std::string> 	normal_directives_set;
		static const std::set<std::string> 	array_directives_set;
		static const std::set<std::string> 	block_directives_set;

		void 		_parse_location_conf(const std::string &cached_string);

	public:
		ServerConfig();
		~ServerConfig();
		ServerConfig(const ServerConfig &ref);
		ServerConfig &operator=(const ServerConfig &ref);
		ServerConfig(Config::directive_container_type directives, const std::string &server_str);

#pragma region Getters

		const std::vector<std::vector<std::string> >	error_page() const;
		const std::vector<std::vector<std::string> >	listen() const;
		const std::map<std::string, LocationConfig>		locations() const;
		const std::set<std::string> 					server_names() const;
		const std::vector<std::string> 					index() const;
		const std::vector<std::string> 					try_files() const;
		const std::vector<std::string> 					client_max_body_size() const;
		const std::vector<std::string> 					index() const;
		const std::vector<std::string> 					error_log() const;
		const std::vector<unsigned int>					ports() const;
		const std::string 								client_body_temp_path() const;
		const std::string			 					autoindex() const;
		const std::string								root() const;

#pragma endregion Getters
};
}

#endif
