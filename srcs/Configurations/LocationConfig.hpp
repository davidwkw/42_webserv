#pragma once

#include <vector>
#include <string>
#include <set>
#include <map>
#include <utility>
#include "../utils/utils.hpp"
#include "CommonServerConfig.hpp"

namespace ft
{

class LocationConfig : public CommonServerConfig
{
	private:
		static const char					*all_directives_array[];
		static const char 					*normal_directives_array[];
		static const char 					*array_directives_array[];

		static const std::set<std::string> 	all_directives_set;
		static const std::set<std::string> 	normal_directives_set;
		static const std::set<std::string> 	array_directives_set;

	public:
		LocationConfig();
		~LocationConfig();
		LocationConfig(const LocationConfig &ref);
		LocationConfig &operator=(const LocationConfig &ref);
		LocationConfig(std::map<std::string, std::string> directives, const std::string &server_str);

		const std::vector<std::string>	client_body_temp_path() const;
};

}
