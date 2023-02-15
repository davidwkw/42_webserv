#include "Config.hpp"

const std::set<std::string> Config::_valid_directives = Config::_fill_valid_directives();

std::set<std::string> Config::_fill_valid_directives(){
	const std::string directives[] =	{
										"server",
										"user",
										"pid",
										"worker_processes",
										"error_log"
										};
	std::set<std::string> fill_set;

	for (std::size_t i = 0; i < sizeof(directives) / sizeof(std::string); i++)
		fill_set.insert(directives[i]);
	return fill_set;
}

Config::Config(void) : _path(){}

Config::~Config(void){}

Config::Config(const Config &ref){
	*this = ref;
}

Config &Config::operator=(const Config &ref){
	if (this != &ref)
	{
		this->_path = ref._path;
		this->_directives = ref._directives;
		this->_servers = ref._servers;
	}
	return *this;
}

Config::Config(const std::string &filename) : _path(filename){
	std::ifstream		conf_stream;
	std::stringstream	ss;
	std::string			raw;
	
	if (this->_open_file(filename, conf_stream) == false) 
		throw std::runtime_error("[Config] Unable to open path " + filename);
	this->_cache_stream(conf_stream, ss);
	conf_stream.close();
	raw = this->_parse_readable_lines(ss);
	this->_parse_server_conf(raw);
}

bool	Config::_open_file(const std::string &filename, std::ifstream& file){
	if (filename.empty() == true){
		return false;
	}
	file.open(filename.c_str());
	return file.is_open();
}

void _cache_stream(std::ifstream& file, std::stringstream &cache_stream){
	cache_stream << file.rdbuf();
}

std::string _parse_readable_lines(std::stringstream &cached_stream){
	std::string	result;
	std::string line;
	std::size_t hash_pos;

	while (std::getline(cached_stream, line)) {
		hash_pos = line.find("#");
        if (line.find("#") != std::string::npos){
			line.erase(hash_pos);
		}
		if (line.find_first_not_of(" \t") == std::string::npos)
			continue;
        result += line + '\n';
    }
	return result;
}

void Config::_parse_server_conf(const std::string &conf_str){
	std::size_t start_index = 0;
	std::size_t delimiter_index = start_index;
	std::string captured_directive;
	std::pair<std::string, std::string> directive_pair;

  	while ((delimiter_index = conf_str.find_first_of(";{}", delimiter_index)) != std::string::npos)
	{
		captured_directive = conf_str.substr(start_index, delimiter_index - start_index);
		if (conf_str[delimiter_index] == ';')
		{
			directive_pair = Config::parse_simple_directive(captured_directive);
		}
		else if (conf_str[delimiter_index] == '{')
		{
			directive_pair.first = trim_ws_str(captured_directive);
			directive_pair.second = str_char_limit_span(conf_str.c_str() + delimiter_index, '{', '}');
			if (directive_pair.second.empty() == true)
				throw std::runtime_error("[Config] Missing } found in configuration file");
			delimiter_index += directive_pair.second.length() + 1;
		}
		else
			throw std::runtime_error("[Config] Found } without { in configuration file");
		if (Config::_directives.find(directive_pair.first) == Config::_directives.end())
			throw std::runtime_error("[Config] No directive matching " + directive_pair.first + " found");
		if (directive_pair.first == "server"){
			std::pair<std::string, Server> server;
			server.second = Server(directive_pair.second);
			server.first = server.second.server_name();
			this->_servers.insert(server);
		}
		else{
			this->_directives.insert(directive_pair);
		}
		++delimiter_index;
		start_index = delimiter_index;
	}
}

const std::string &Config::path(void) const{
	return this->_path;
}

std::pair<std::string, std::string> Config::parse_simple_directive(const std::string &directive){
	std::stringstream temp(directive);
	std::string k;
	std::string v;
	std::string word;

	temp >> k;
	v = trim_ws_str(temp.str());
	
	return std::make_pair(k, v);
}
