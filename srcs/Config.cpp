#include "Config.hpp"

const std::set<std::string> Config::_directives = Config::_fill_directives();

std::set<std::string> Config::_fill_directives(){
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
	this->_parse_readable_lines(ss);
	this->_parse_server_conf(ss);
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

void _parse_readable_lines(std::stringstream &cached_stream){
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

void Config::_parse_server_conf(const std::stringstream &cached_stream){
	std::string conf_str = cached_stream.str();
	std::size_t start_index = 0; // need to name it better...it's index of the start of the string before delimiter
	std::size_t delimiter_index = start_index;
	std::string captured_directive;

  	while ((delimiter_index = conf_str.find_first_of(";{}", delimiter_index)) != std::string::npos)
	{
		captured_directive = conf_str.substr(start_index, delimiter_index - start_index);
		if (conf_str[delimiter_index] == ';')
		{
			// do something
			Config::parse_simple_directive(captured_directive);
		}
		else if (conf_str[delimiter_index] == '{')
		{
			Config::_directives.find();
			// do something else
		}
		else
		{
			// } closing brace
		}
		++delimiter_index;
		start_index = delimiter_index;
	}
}

const std::string &Config::path(void) const{
	return this->_path;
}

const char* Config::UnableToOpenPath::what() const throw()
{
	return ("Failed to open config file path");
}

std::pair<std::string, std::string> Config::parse_simple_directive(const std::string &directive){
	std::stringstream temp(directive);
	std::string k;
	std::string v;
	std::string word;

	temp >> k;
	while (temp >> word)
		v += (v.empty() ? "" :  " " ) + word;
	
	return std::make_pair(k, v);
}
