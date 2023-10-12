#pragma once

#include <vector>
#include <set>
#include <string>
#include <algorithm>
#include "../utils/utils.hpp"
#include "Config.hpp"

namespace ft 
{

class CommonServerConfig : public Config
{
	public:
		CommonServerConfig();
		~CommonServerConfig();
		CommonServerConfig(const CommonServerConfig &ref);
		CommonServerConfig &operator=(const CommonServerConfig &ref);
		CommonServerConfig(const std::map<std::string, std::string> &directives);
		CommonServerConfig(const std::map<std::string, std::string> &directives, std::set<std::string> inclusion_set);

		std::vector<std::string> 				try_files() const;
		std::vector<std::string>				error_log() const;
		std::set<std::string>					limit_except() const;
		std::vector<std::vector<std::string> >	error_page() const;
		std::vector<std::string>				index() const;
		std::string								root() const;
		std::string								autoindex() const;
		std::vector<std::string>				redirect() const;
		std::vector<std::vector<std::string> >	cgi() const;
		std::size_t 							client_max_body_size() const;

};

}
