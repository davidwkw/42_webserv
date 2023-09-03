#include "LocationConfig.hpp"

namespace ft
{

const char *LocationConfig::all_directives_array[] =	{
														"error_log",
														"index",
														"error_page",
														"redirect",
														"cgi_bin",
														"autoindex",
														"root",
														"client_body_temp_path",
														"try_files",
														"client_max_body_size",
														"limit_except"
														};

const char *LocationConfig::normal_directives_array[] =	{
														"index",
														"redirect",
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
														"cgi_bin",
														};
									
// const char *LocationConfig::block_directives_array[] = 	{
// 														"limit_except"
// 														};

const std::set<std::string> LocationConfig::all_directives_set = init_string_set(LocationConfig::all_directives_array);

const std::set<std::string> LocationConfig::normal_directives_set = init_string_set(LocationConfig::normal_directives_array);

const std::set<std::string> LocationConfig::array_directives_set = init_string_set(LocationConfig::array_directives_array);

// const std::set<std::string> LocationConfig::block_directives_set = init_string_set(LocationConfig::block_directives_array);

LocationConfig::LocationConfig(void) : CommonServerConfig(){}

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

LocationConfig::LocationConfig(Config::directive_container_type directives, const std::string &location_str) : CommonServerConfig(directives, LocationConfig::all_directives_set)
{
	this->_directives = Config::parse_all_directives(location_str, LocationConfig::all_directives_set);
}

#pragma region Getters

const std::vector<std::string> LocationConfig::client_body_temp_path() const
{
	return this->find_normal_directive("client_body_temp_path");
}

#pragma endregion Getters

}
