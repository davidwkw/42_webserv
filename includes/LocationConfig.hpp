#ifndef __LOCATION_CONFIG_HPP__
#define __LOCATION_CONFIG_HPP__

#include <vector>
#include <string>
#include <set>
#include <map>
#include <utility>
#include "BaseConfig.hpp"

class LocationConfig : public BaseConfig{
	private:
		static const char					*all_directives_array[];
		static const char 					*normal_directives_array[];
		static const char 					*array_directives_array[];
		static const char 					*block_directives_array[];

		static const std::set<std::string> 	all_directives_set;
		static const std::set<std::string> 	normal_directives_set;
		static const std::set<std::string> 	array_directives_set;
		static const std::set<std::string> 	block_directives_set;

		void 		_parse_limit_except(const std::string &cached_string);

	public:
		LocationConfig();
		~LocationConfig();
		LocationConfig(const LocationConfig &ref);
		LocationConfig &operator=(const LocationConfig &ref);
		LocationConfig(BaseConfig::directive_container_type directives, const std::string &server_str);

		const std::vector<std::vector<std::string> > error_page() const;
		const std::vector<std::string> index() const;
		const std::vector<std::string> client_body_temp_path() const;
		const std::vector<std::string> root() const;
		const std::vector<std::string> try_files() const;
		const std::vector<std::string> client_max_body_size() const;
		const std::vector<std::string> autoindex() const;
		const std::vector<std::string> index() const;
		const std::vector<std::string> error_log() const;
};

#endif
