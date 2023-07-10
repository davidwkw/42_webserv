#include "LocationConfig.hpp"

namespace ft
{

const char *LocationConfig::all_directives_array[] =	{
														"error_log",
														"index",
														"error_page",
														"return",
														"cgi",
														"autoindex",
														"root",
														"client_body_temp_path",
														"try_files",
														"client_max_body_size",
														"limit_except"
														};

const char *LocationConfig::normal_directives_array[] =	{
														"index",
														"return",
														"cgi",
														"autoindex",
														"root",
														"client_body_temp_path",
														"try_files",
														"client_max_body_size",
														"limit_except"
														};

const char *LocationConfig::array_directives_array[] =	{
														"error_page",
														"error_log",
														};
									
// const char *LocationConfig::block_directives_array[] = 	{
// 														"limit_except"
// 														};

const std::set<std::string> LocationConfig::all_directives_set = init_string_set(LocationConfig::all_directives_array);

const std::set<std::string> LocationConfig::normal_directives_set = init_string_set(LocationConfig::normal_directives_array);

const std::set<std::string> LocationConfig::array_directives_set = init_string_set(LocationConfig::array_directives_array);

// const std::set<std::string> LocationConfig::block_directives_set = init_string_set(LocationConfig::block_directives_array);

LocationConfig::LocationConfig(void) : Config(){}

LocationConfig::~LocationConfig(void){}

LocationConfig::LocationConfig(const LocationConfig &ref)
{
	*this = ref;
}

LocationConfig &LocationConfig::operator=(const LocationConfig &ref)
{
	if (this != &ref)
	{
		this->_directives = ref._directives;
	}
	return *this;
}

LocationConfig::LocationConfig(Config::directive_container_type directives, const std::string &location_str) : Config(directives, LocationConfig::all_directives_set)
{
	this->_directives = Config::parse_all_directives(location_str, LocationConfig::all_directives_set);
}

const std::vector<std::vector<std::string> > LocationConfig::error_page() const
{
	return this->find_array_directive("error_page");
}


const std::vector<std::string> LocationConfig::try_files() const
{
	return this->find_normal_directive("try_files");
}

const std::vector<std::string> LocationConfig::client_max_body_size() const
{
	return this->find_normal_directive("client_max_body_size");
}

const std::vector<std::string> LocationConfig::index() const
{
	return this->find_normal_directive("index");
}

const std::vector<std::string> LocationConfig::error_log() const
{
	return this->find_normal_directive("error_log");
}

const std::set<std::string>	LocationConfig::limit_except() const
{
	return std::set<std::string>(this->find_normal_directive("limit_except").begin(), this->find_normal_directive("limit_except").end());
}

const std::vector<std::string> LocationConfig::client_body_temp_path() const
{
	return this->find_normal_directive("client_body_temp_path");
}

const std::string LocationConfig::root() const
{
	std::vector<std::string> temp_vect = this->find_normal_directive("root");
	if (temp_vect.size() != 0)
		return temp_vect.front();
	return "public";
}

const std::string LocationConfig::autoindex() const
{
	std::vector<std::string> temp_vect = this->find_normal_directive("autoindex");
	if (temp_vect.size() != 0)
		return temp_vect.front();
	return "";
}

}
