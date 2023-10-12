#include "CommonServerConfig.hpp"

namespace ft
{

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

CommonServerConfig::CommonServerConfig(const std::map<std::string, std::string> &directives) : Config(directives)
{}

CommonServerConfig::CommonServerConfig(const std::map<std::string, std::string> &directives, std::set<std::string> inclusion_set) : Config(directives, inclusion_set)
{}

std::vector<std::vector<std::string> > CommonServerConfig::error_page() const
{
	return this->find_array_directive("error_page");
}

std::vector<std::string> CommonServerConfig::try_files() const
{
	return this->find_normal_directive("try_files");
}

std::vector<std::string> CommonServerConfig::index() const
{
	return this->find_normal_directive("index");
}

std::vector<std::string> CommonServerConfig::error_log() const
{
	return this->find_normal_directive("error_log");
}

std::set<std::string>	CommonServerConfig::limit_except() const
{
	return std::set<std::string>(this->find_normal_directive("limit_except").begin(), this->find_normal_directive("limit_except").end());
}

std::string CommonServerConfig::root() const
{
	std::vector<std::string> temp_vect = this->find_normal_directive("root");
	if (temp_vect.size() != 0)
		return temp_vect.front();
	return "public";
}

std::string CommonServerConfig::autoindex() const
{
	std::vector<std::string> temp_vect = this->find_normal_directive("autoindex");
	if (temp_vect.size() != 0)
		return temp_vect.front();
	return "off";
}

std::size_t CommonServerConfig::client_max_body_size() const // validation shouldn't be done in the getter, but no input validition done prior to this call so no choice at the moment.
{
	std::vector<std::string> temp_vect = this->find_normal_directive("client_max_body_size");
	std::string value_string;
	std::stringstream ss;
	std::size_t value;
	std::size_t alpha_index;
	if (temp_vect.size() != 0)
		value_string =  temp_vect.front();
	else
		value_string = "1m";
	std::transform(value_string.begin(), value_string.end(), value_string.begin(), static_cast<int (*)(int)>(std::tolower));
	alpha_index = value_string.find_first_not_of("1234567890");	
	ss << value_string.substr(0, alpha_index);
	if (value_string.substr(alpha_index).length() > 1 || std::string("kmg").find(value_string[alpha_index]) == std::string::npos)
	{
		std::runtime_error("Invalid max body size value");
	}
	if (value_string[alpha_index] == 'k')
		ss << "000";
	else if (value_string[alpha_index] == 'm')
		ss << "000000";
	else if (value_string[alpha_index] == 'g')
		ss << "000000000";
	ss >> value;
	return value;
}

std::vector<std::string> CommonServerConfig::redirect() const
{
	std::vector<std::string> temp_vect = this->find_normal_directive("redirect");
	if (temp_vect.size() == 1)
	{
		temp_vect.insert(temp_vect.begin(), "301");
	}
	return temp_vect;
}

std::vector<std::vector<std::string> > CommonServerConfig::cgi() const
{
	return this->find_array_directive("cgi_bin");
}

}
