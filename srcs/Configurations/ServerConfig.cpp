#include "ServerConfig.hpp"

namespace ft
{

const char *ServerConfig::all_directives_array[] = {
													"error_log",
													"server_name",
													"listen",
													"index",
													"error_page",
													"redirect",
													"cgi_bin",
													"autoindex",
													"root",
													"client_body_temp_path",
													"try_files",
													"client_max_body_size"
													};

const char *ServerConfig::normal_directives_array[] =	{
														"index",
														"redirect",
														"autoindex",
														"root",
														"client_body_temp_path",
														"try_files",
														"client_max_body_size",
														"limit_except"
														};

const char *ServerConfig::array_directives_array[] = {
													"error_page",
													"error_log",
													"server_name",
													"cgi_bin",
													"listen"
													};
									
const char *ServerConfig::block_directives_array[] =	{
														"location"
														};

const std::set<std::string> ServerConfig::all_directives_set = init_string_set(ServerConfig::all_directives_array);

const std::set<std::string> ServerConfig::normal_directives_set = init_string_set(ServerConfig::normal_directives_array);

const std::set<std::string> ServerConfig::array_directives_set = init_string_set(ServerConfig::array_directives_array);

const std::set<std::string> ServerConfig::block_directives_set = init_string_set(ServerConfig::block_directives_array);

ServerConfig::ServerConfig(void) : CommonServerConfig(), _locations(){}

ServerConfig::~ServerConfig(void){}

ServerConfig::ServerConfig(const ServerConfig &ref) : CommonServerConfig(), _locations(ref._locations)
{
	*this = ref;
}

ServerConfig &ServerConfig::operator=(const ServerConfig &ref)
{
	if (this != &ref)
	{
		this->_directives = ref._directives;
		this->_locations = ref._locations;
	}
	return *this;
}

ServerConfig::ServerConfig(std::map<std::string, std::string> directives, const std::string &server_str) : CommonServerConfig()
{
	this->_directives = Config::parse_all_directives(server_str, ServerConfig::all_directives_set);
	this->_parse_location_conf(server_str);
	Config::fill_directives(this->_directives, ServerConfig::all_directives_set, directives);
	Config::fill_directives(this->_directives, ServerConfig::all_directives_set, Config::directive_defaults);
}

void 	ServerConfig::_parse_location_conf(const std::string &cached_string){
	std::multimap<std::string, std::string>	block_directives;
	std::vector<std::string>				identifier_tokens;
	std::pair<std::string, LocationConfig>	block_pair;

	block_directives = Config::parse_block_directives(cached_string);
	for (std::multimap<std::string, std::string>::iterator it = block_directives.begin(); it != block_directives.end(); ++it){
		identifier_tokens = tokenise_str(it->first);
		if (identifier_tokens.size() != 2)
		{
			throw std::runtime_error("[ServerConfig] Invalid number of identifiers for this context");
		}
		else if (identifier_tokens[0] != "location")
		{
			throw std::runtime_error("[ServerConfig] Invalid block directive for this context");
		}
		else
		{
			block_pair.first = identifier_tokens[1];
			block_pair.second = LocationConfig(this->_directives, it->second);
			this->_locations.insert(block_pair);
		}
	}
}

std::map<std::string, LocationConfig>	ServerConfig::locations() const
{
	return this->_locations;
}

std::set<std::string> ServerConfig::server_names() const
{
	std::vector<std::vector<std::string> >	temp;
	std::set<std::string>					result;
	
	temp = this->find_array_directive("server_name");
	for (std::vector<std::vector<std::string> >::iterator it = temp.begin(); it != temp.end(); ++it)
	{
		for (std::vector<std::string>::iterator it2 = it->begin(); it2 != it->end(); ++it2)
		{
			result.insert(*it2);
		}
	}
	return result;
}

std::vector<std::vector<std::string> > ServerConfig::listen() const
{	
	return this->find_array_directive("listen");
}

std::string ServerConfig::client_body_temp_path() const
{
	std::vector<std::string> temp_vect = this->find_normal_directive("client_body_temp_path");
	if (temp_vect.size() != 0)
		return temp_vect.front();
	return "client_body_temp";
}

std::vector<unsigned int> ServerConfig::ports() const
{
	std::vector<std::vector<std::string> >	listen_vect;
	std::vector<unsigned int> ret_vector;

	listen_vect = this->listen();
	for (std::vector<std::vector<std::string> >::iterator it = listen_vect.begin(); it != listen_vect.end(); it++)
	{
		std::vector<std::string> &port = *it;

		ret_vector.push_back(std::strtoul(port.front().c_str(), NULL, 10));
	}
	return ret_vector;
}

std::map<std::string, std::string> ServerConfig::directives() const
{
	return this->_directives;
}

}
