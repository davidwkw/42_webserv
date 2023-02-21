#ifndef __BASE_CONFIG_HPP__
#define __BASE_CONFIG_HPP__

#include <map>
#include <string>

class BaseConfig{
	protected:
		std::map<std::string, std::string> _directives;
	public:
		BaseConfig();
		virtual ~BaseConfig();
		BaseConfig(const BaseConfig &ref);
		BaseConfig &operator=(const BaseConfig &ref);
		BaseConfig(const std::map<std::string,std::string> &directives);

		std::map<std::string, std::string> directives() const;
		void add_directive(std::pair<std::string, std::string> directive);
};

#endif
