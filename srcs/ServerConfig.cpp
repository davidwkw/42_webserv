#include "ServerConfig.hpp"

const std::set<std::string> ServerConfig::_valid_directives = ServerConfig::_fill_valid_directives();

std::set<std::string> ServerConfig::_fill_valid_directives(){
	const std::string directives[] =	{
										"root",
										"autoindex",
										"index",
										"return",
										"try_files",
										"upload_store"
										};
	std::set<std::string> fill_set;
	for (std::size_t i = 0; i < sizeof(directives) / sizeof(std::string); i++)
		fill_set.insert(directives[i]);
	return fill_set;
}

ServerConfig::ServerConfig(void) : _directives(), _locations() {

}

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

ServerConfig::ServerConfig(const std::string &server_str){
	std::size_t start_index = 0;
	std::size_t delimiter_index = start_index;
	std::string captured_directive;
	std::pair<std::string, std::string> directive_pair;

  	while ((delimiter_index = server_str.find_first_of(";{}", delimiter_index)) != std::string::npos)
	{
		captured_directive = server_str.substr(start_index, delimiter_index - start_index);
		if (server_str[delimiter_index] == ';')
		{
			directive_pair = Config::parse_simple_directive(captured_directive);
		}
		else if (server_str[delimiter_index] == '{')
		{
			directive_pair.first = trim_ws_str(captured_directive);
			directive_pair.second = str_char_limit_span(server_str.c_str() + delimiter_index, '{', '}');
			if (directive_pair.second.empty() == true)
				throw std::runtime_error("[Config] Missing } found in configuration file");
			delimiter_index += directive_pair.second.length() + 1;
		}
		else
			throw std::runtime_error("[Config] Found } without { in configuration file");
		if (ServerConfig::_valid_directives.find(directive_pair.first) == ServerConfig::_valid_directives.end())
			throw std::runtime_error("[Config] No directive matching " + directive_pair.first + " found");
		if (directive_pair.first == "location"){
			std::pair<std::string, Location> location;
			location.second = Location(directive_pair.second);
			location.first = location.second.path();
			this->_locations.insert(location);
		}
		else{
			this->_directives.insert(directive_pair);
		}
		++delimiter_index;
		start_index = delimiter_index;
	}
}

void ServerConfig::_init_directives() {

	this->_directives.insert(std::make_pair("server_name", "_"));
	this->_directives.insert(std::make_pair("root", "html"));
	this->_directives.insert(std::make_pair("autoindex", "off"));
	// this->_directives.insert(std::make_pair("limit_except", "")); location directive
	this->_directives.insert(std::make_pair("listen", "*:80 | *:8000"));
	this->_directives.insert(std::make_pair("error_page", ""));
	this->_directives.insert(std::make_pair("index", "index.html"));
	this->_directives.insert(std::make_pair("upload_store", ""));
}
