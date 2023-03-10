#include "Config.hpp"

const char *Config::all_directives_array[] =	{
												"user",
												"pid",
												"worker_processes",
												"error_log"
												};

const char *Config::normal_directives_array[] =	{
												"user",
												"pid",
												"worker_processes",
												};

const char *Config::array_directives_array[] =	{
												"error_log",
												};
									
const char *Config::block_directives_array[] = 	{
												"server"
												};

const std::set<std::string> Config::all_directives_set = init_string_set(Config::all_directives_array);

const std::set<std::string> Config::normal_directives_set = init_string_set(Config::normal_directives_array);

const std::set<std::string> Config::array_directives_set = init_string_set(Config::array_directives_array);

const std::set<std::string> Config::block_directives_set = init_string_set(Config::block_directives_array);

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
	this->_directives = BaseConfig::parse_all_directives(raw, Config::all_directives_set);
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
	std::map<std::string, std::string> block_directives;
	std::pair<long, ServerConfig> block_pair;

	block_directives = BaseConfig::parse_block_directives(conf_str);
	for (directive_container_type::iterator it = block_directives.begin(); it != block_directives.end(); ++it){
		if (it->first == "server"){
			block_pair.first = this->_servers.size();
			block_pair.second = ServerConfig(this->_directives, it->second);
			this->_servers.insert(block_pair);
		}
		else
			throw std::runtime_error("[Config] Invalid block directive for this context");
	}
}

const std::string &Config::path(void) const{
	return this->_path;
}
