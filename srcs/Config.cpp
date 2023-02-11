#include "Config.hpp"

const std::set<std::string> Config::directives = {
												"server",
												"user",
												"pid",
												"worker_processes",
												"error_log"
												};

Config::Config(void) : _path(){}

Config::~Config(void){}

Config::Config(const Config &ref){
	*this = ref;
}

Config &Config::operator=(const Config &ref){
	if (this != &ref)
	{

	}
	return *this;
}

Config::Config(const std::string &filename) : _path(filename){
	std::ifstream		conf_stream;
	std::stringstream	ss;
	
	if (this->_open_file(filename, conf_stream) == false) 
		throw UnableToOpenPath(); // consider throwing a more formatted error message? i.e [Config]: message
	this->_cache_stream(conf_stream, ss);
	conf_stream.close(); // not necessary, but can close earlier to release resources before moving out of constructor scope
	this->_remove_comments(ss);

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

void _remove_comments(std::stringstream &cached_stream){
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
	cached_stream.str(result); // does the eof bit reset? might need to test
}

void Config::_parse_server_conf(std::stringstream &cached_stream){
	std::string line;

	while(std::getline(cached_stream, line, '{')){

	}
}

const std::string &Config::path(void) const{
	return this->_path;
}

const char* Config::UnableToOpenPath::what() const throw()
{
	return ("Failed to open config file path");
}
