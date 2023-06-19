#ifndef __SERVER_CONFIG_HPP__
#define __SERVER_CONFIG_HPP__

#include <set>
#include <map>
#include <sstream>
#include "LocationConfig.hpp"
#include "BaseConfig.hpp"
#include "Config.hpp"
#include "webserv.hpp"

namespace ft
{

class ServerConfig : public BaseConfig{
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
		ServerConfig(BaseConfig::directive_container_type directives, const std::string &server_str);

#pragma region Getters

		const std::map<std::string, LocationConfig> &locations() const;
		const std::vector<std::string> server_names() const;
		const std::vector<std::vector<std::string> > listen() const;
		const std::vector<std::vector<std::string> > error_page() const;
		const std::vector<std::string> index() const;
		const std::vector<std::string> client_body_temp_path() const;
		const std::vector<std::string> try_files() const;
		const std::vector<std::string> client_max_body_size() const;
		const std::vector<std::string> autoindex() const;
		const std::vector<std::string> index() const;
		const std::vector<std::string> error_log() const;
		const std::string root() const;
		const std::vector<int> ports() const;

#pragma endregion Getters
};
}

#endif
