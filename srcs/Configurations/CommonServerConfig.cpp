#include "CommonServerConfig.hpp"

namespace ft
{

#pragma region Constructors

CommonServerConfig::CommonServerConfig() : Config(){}

CommonServerConfig::~CommonServerConfig(){}

CommonServerConfig::CommonServerConfig(const CommonServerConfig &ref) : Config()
{
	*this = ref;
}

CommonServerConfig &CommonServerConfig::operator=(const CommonServerConfig &ref)
{
	if (this != &ref)
	{
		this->_directives = ref._directives;
	}
	return *this;
}

CommonServerConfig::CommonServerConfig(const directive_container_type &directives) : Config(directives)
{}

CommonServerConfig::CommonServerConfig(const directive_container_type &directives, allowed_directives_container_type inclusion_set) : Config(directives, inclusion_set)
{}

#pragma endregion Constructors

#pragma region Getters

const std::vector<std::vector<std::string> > CommonServerConfig::error_page() const
{
	return this->find_array_directive("error_page");
}

const std::vector<std::string> CommonServerConfig::try_files() const
{
	return this->find_normal_directive("try_files");
}

const std::vector<std::string> CommonServerConfig::index() const
{
	return this->find_normal_directive("index");
}

const std::vector<std::string> CommonServerConfig::error_log() const
{
	return this->find_normal_directive("error_log");
}

const std::set<std::string>	CommonServerConfig::limit_except() const
{
	return std::set<std::string>(this->find_normal_directive("limit_except").begin(), this->find_normal_directive("limit_except").end());
}

const std::string CommonServerConfig::root() const
{
	std::vector<std::string> temp_vect = this->find_normal_directive("root");
	if (temp_vect.size() != 0)
		return temp_vect.front();
	return "/public/";
}

const std::string CommonServerConfig::autoindex() const
{
	std::vector<std::string> temp_vect = this->find_normal_directive("autoindex");
	if (temp_vect.size() != 0)
		return temp_vect.front();
	return "off";
}

#pragma region Getters

}
