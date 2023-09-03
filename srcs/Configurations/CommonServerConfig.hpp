#pragma once

#include "Config.hpp"

namespace ft 
{

class CommonServerConfig : public Config
{
	public:
		CommonServerConfig();
		~CommonServerConfig();
		CommonServerConfig(const CommonServerConfig &src);
		CommonServerConfig &operator=(const CommonServerConfig &ref);
		CommonServerConfig(const directive_container_type &directives);
		CommonServerConfig(const directive_container_type &directives, allowed_directives_container_type inclusion_set);

		const std::vector<std::string> 					try_files() const;
		const std::vector<std::string>					error_log() const;
		const std::set<std::string>						limit_except() const;
		const std::vector<std::vector<std::string> >	error_page() const;
		const std::vector<std::string>					index() const;
		const std::string								root() const;
		const std::string								autoindex() const;
		const std::size_t 								client_max_body_size() const;
		const std::vector<std::string>					redirect() const;
		const std::vector<std::vector<std::string> >	cgi() const;

	private:
		Config *_config;
};

}
