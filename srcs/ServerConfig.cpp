#include "ServerConfig.hpp"

const char *ServerConfig::all_directives_array[] = {
													"error_log",
													"server_name",
													"listen"
													"index",
													"error_page",
													"listen",
													"return",
													"cgi",
													"autoindex",
													"root",
													"client_body_temp_path",
													"try_files",
													"client_max_body_size"
													};

const char *ServerConfig::normal_directives_array[] =	{
														"index",
														"error_page",
														"return",
														"cgi",
														"autoindex",
														"root",
														"client_body_temp_path",
														"try_files",
														"client_max_body_size"
														};

const char *ServerConfig::array_directives_array[] = {
													"error_log",
													"server_name",
													"listen"
													};
									
const char *ServerConfig::block_directives_array[] =	{
														"location"
														};

const std::set<std::string> ServerConfig::all_directives_set = init_string_set(ServerConfig::all_directives_array);

const std::set<std::string> ServerConfig::normal_directives_set = init_string_set(ServerConfig::normal_directives_array);

const std::set<std::string> ServerConfig::array_directives_set = init_string_set(ServerConfig::array_directives_array);

const std::set<std::string> ServerConfig::block_directives_set = init_string_set(ServerConfig::block_directives_array);

ServerConfig::ServerConfig(void) : BaseConfig(), _locations(){}

ServerConfig::~ServerConfig(void){}

ServerConfig::ServerConfig(const ServerConfig &ref){
	*this = ref;
}

ServerConfig &ServerConfig::operator=(const ServerConfig &ref){
	if (this != &ref)
	{
		this->_directives = ref._directives;
		this->_locations = ref._locations;
	}
	return *this;
}

ServerConfig::ServerConfig(BaseConfig::directive_container_type directives, const std::string &location_str) : BaseConfig(directives, ServerConfig::all_directives_set) {
	this->_directives = BaseConfig::parse_all_directives(location_str, ServerConfig::all_directives_set);
	this->_parse_location_conf(location_str);
}

void 	ServerConfig::_parse_location_conf(const std::string &cached_string){
	std::map<std::string, std::string> block_directives;
	std::vector<std::string> identifier_tokens;
	std::pair<std::string, LocationConfig> block_pair;

	block_directives = BaseConfig::parse_block_directives(cached_string);
	for (std::map<std::string, std::string>::iterator it = block_directives.begin(); it != block_directives.end(); ++it){
		identifier_tokens = tokenise_str(it->first);
		if (identifier_tokens.size() != 2)
			throw std::runtime_error("[ServerConfig] Invalid number of identifiers for this context");
		else if (identifier_tokens[0] != "location")
			throw std::runtime_error("[ServerConfig] Invalid block directive for this context");
		else
		{
			block_pair.first = identifier_tokens[1];
			block_pair.second = LocationConfig(this->_directives, it->second);
			this->_locations.insert(block_pair);
		}
	}
}

const std::vector<std::string> ServerConfig::server_names() const{
	std::vector<std::vector<std::string> > temp;
	std::vector<std::string> ret_vector;
	
	temp = this->find_array_directive("server_name");
	for (std::vector<std::vector<std::string> >::iterator it = temp.begin(); it != temp.end(); ++it){
		for (std::vector<std::string>::iterator it2 = it->begin(); it2 != it->end(); ++it2){
			ret_vector.push_back(*it2);
		}
	}
	return ret_vector;
}

const std::vector<std::vector<std::string> > ServerConfig::error_page() const{
	return this->find_array_directive("error_page");
}

const std::vector<std::string> ServerConfig::client_body_temp_path() const{
	return this->find_normal_directive("client_body_temp_path");
}

const std::vector<std::string> ServerConfig::root() const{
	return this->find_normal_directive("root");
}

const std::vector<std::string> ServerConfig::try_files() const{
	return this->find_normal_directive("try_files");
}

const std::vector<std::vector<std::string> > ServerConfig::listen() const{
	return this->find_array_directive("listen");
}

const std::vector<std::string> ServerConfig::client_max_body_size() const{
	return this->find_normal_directive("client_max_body_size");
}

const std::vector<std::string> ServerConfig::autoindex() const{
	return this->find_normal_directive("autoindex");
}

const std::vector<std::string> ServerConfig::index() const{
	return this->find_normal_directive("index");
}

const std::vector<std::string> ServerConfig::error_log() const{
	return this->find_normal_directive("error_log");
}
