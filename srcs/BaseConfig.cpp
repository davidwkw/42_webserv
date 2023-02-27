#include "BaseConfig.hpp"

const char *BaseConfig::simple_directive_array[] = {
											"server",
											"location",
											"error_log",
											"server_name",
											"index",
											"error_page",
											"listen",
											"return",
											"cgi",
											"user",
											"pid",
											"worker_processes",
											"autoindex",
											"root",
											"client_body_temp_path",
											"try_files",
											"client_max_body_size",
											"limit_except"
											};
									
const char *BaseConfig::block_directive_array[] =	{
													"server",
													"location",
													"limit_except"
													};

const char *BaseConfig::array_directive_array[] =	{
													"error_log",
													"server_name",
													"listen"
													};


const std::set<std::string> BaseConfig::_simple_directives_set = BaseConfig::_init_directive_set(BaseConfig::simple_directive_array);

const std::set<std::string> BaseConfig::_array_directives_set = BaseConfig::_init_directive_set(BaseConfig::array_directive_array);

const std::set<std::string> BaseConfig::_block_directives_set = BaseConfig::_init_directive_set(BaseConfig::block_directive_array);

const std::map<std::string, BaseConfig::Actions> BaseConfig::_directive_actions = BaseConfig::_fill_directive_actions();

std::map<std::string, BaseConfig::Actions> BaseConfig::_fill_directive_actions(){
	typedef std::pair<std::string, BaseConfig::Actions> directive_action_pair;
	
	const directive_action_pair directives_actions[]	=	{
												directive_action_pair("server_name", APPEND_EXISTING),
												directive_action_pair("error_log", APPEND_EXISTING),
												directive_action_pair("index", APPEND_EXISTING),
												directive_action_pair("error_page", INSERT_NEW), // needs more testing..
												directive_action_pair("listen", INSERT_NEW), // but will throw error if a port is already defined?
												directive_action_pair("return", INSERT_NEW), // will process the first return
												directive_action_pair("cgi", INSERT_NEW),
												directive_action_pair("user", DUPLICATE_ERROR),
												directive_action_pair("pid", DUPLICATE_ERROR),
												directive_action_pair("worker_processes", DUPLICATE_ERROR),
												directive_action_pair("autoindex", DUPLICATE_ERROR),
												directive_action_pair("root", DUPLICATE_ERROR),
												directive_action_pair("client_body_temp_path", DUPLICATE_ERROR),
												directive_action_pair("try_files", DUPLICATE_ERROR),
												directive_action_pair("client_max_body_size", DUPLICATE_ERROR),
												directive_action_pair("limit_except", DUPLICATE_ERROR)
												};

	std::map<std::string, BaseConfig::Actions> fill_map;

	for (std::size_t i = 0; i < sizeof(directives) / sizeof(std::string); i++)
		fill_map.insert(directives_actions[i]);
	return fill_map;
}

const std::map<std::string, std::string> BaseConfig::directive_defaults = BaseConfig::_init_directive_defaults();

std::map<std::string, std::string> BaseConfig::_init_directive_defaults(){
	typedef std::pair<std::string, std::string> directive_default_pair;
	
	const directive_default_pair directives_actions[]	=	{
												directive_default_pair("server_name", ""),
												directive_default_pair("error_log", "logs/error.log error"),
												directive_default_pair("index", "index.html"),
												directive_default_pair("listen", "*:80 | *:8000"),
												directive_default_pair("user", "nobody nobody"),
												directive_default_pair("pid", "logs/nginx.pid"),
												directive_default_pair("worker_processes", "1"),
												directive_default_pair("autoindex", "off"),
												directive_default_pair("root", "html"),
												directive_default_pair("client_body_temp_path", "client_body_temp"),
												directive_default_pair("client_max_body_size", "1m")
												};

	std::map<std::string, std::string> fill_map;

	for (std::size_t i = 0; i < sizeof(directives) / sizeof(std::string); i++)
		fill_map.insert(directives_actions[i]);
	return fill_map;
}

BaseConfig::BaseConfig() : _directives(){}

BaseConfig::~BaseConfig(){}

BaseConfig::BaseConfig(const BaseConfig &ref) : _directives(ref._directives){
	*this = ref;
}

BaseConfig &BaseConfig::operator=(const BaseConfig &ref){
	if (this != &ref){
		this->_directives = ref._directives;
	}
	return *this;
}

BaseConfig::BaseConfig(const directive_container_type &directives){
	this->_directives = directives;
}

BaseConfig::directive_container_type BaseConfig::directives() const{
	return this->_directives;
}

void BaseConfig::add_directive(directive_type directive){
	this->_directives.insert(directive);
}

BaseConfig::directive_type BaseConfig::parse_simple_directive(const std::string &directive, BaseConfig::DirectiveStatus status = DEFINED){
	std::stringstream 				temp;
	std::string 					k;
	std::string 					word;
	BaseConfig::directive_value_type result_value;

	temp.str(trim_ws_str(directive));
	temp >> k;
	while (temp >> word){
		result_value.first += (result_value.first.empty() ? "" :  " " ) + word;
	}
	result_value.second = status;
	
	return std::make_pair(k, result_value);
}
