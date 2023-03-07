#include "ServerConfig.hpp"

// const std::set<std::string> ServerConfig::_valid_directives = ServerConfig::_fill_valid_directives();

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
									
const char *ServerConfig::block_directives_array[] = 	{
														"location"
														};

const std::set<std::string> ServerConfig::all_directives_set = BaseConfig::_init_directive_set(ServerConfig::all_directives_array);

const std::set<std::string> ServerConfig::normal_directives_set = BaseConfig::_init_directive_set(ServerConfig::normal_directives_array);

const std::set<std::string> ServerConfig::array_directives_set = BaseConfig::_init_directive_set(ServerConfig::array_directives_array);

const std::set<std::string> ServerConfig::block_directives_set = BaseConfig::_init_directive_set(ServerConfig::block_directives_array);

ServerConfig::ServerConfig(void) : BaseConfig(), _locations() {}

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

ServerConfig::ServerConfig(BaseConfig::directive_container_type directives, const std::string &server_str) : BaseConfig(directives, ServerConfig::all_directives_set) {
	directive_container_type parsed_directives_container;

	parsed_directives_container = BaseConfig::parse_all_directives(server_str);
	for (directive_container_type::iterator it = parsed_directives_container.begin(); it != parsed_directives_container.end(); ++it)
	{
		if (ServerConfig::all_directives_set.find(it->first) == ServerConfig::all_directives_set.end())
			throw std::runtime_error("[ServerConfig] " + it->first + " is invalid for this context");
		this->_directives[it->first] = it->second;
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
