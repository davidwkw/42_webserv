#include "WebserverConfig.hpp"

namespace ft
{

const char *WebserverConfig::all_directives_array[] =	{
												"user",
												"pid",
												"worker_processes",
												"error_log"
												};

const char *WebserverConfig::normal_directives_array[] =	{
												"user",
												"pid",
												"worker_processes",
												};

const char *WebserverConfig::array_directives_array[] =	{
												"error_log",
												};
									
const char *WebserverConfig::block_directives_array[] = 	{
												"server"
												};

const std::set<std::string> WebserverConfig::all_directives_set = init_string_set(WebserverConfig::all_directives_array);

const std::set<std::string> WebserverConfig::normal_directives_set = init_string_set(WebserverConfig::normal_directives_array);

const std::set<std::string> WebserverConfig::array_directives_set = init_string_set(WebserverConfig::array_directives_array);

const std::set<std::string> WebserverConfig::block_directives_set = init_string_set(WebserverConfig::block_directives_array);


WebserverConfig::WebserverConfig(void) : Config(), _path(){}

WebserverConfig::~WebserverConfig(void){}

WebserverConfig::WebserverConfig(const WebserverConfig &ref) : Config(), _path(), _servers(), _port_server_config_index_map()
{
	*this = ref;
}

WebserverConfig::WebserverConfig(const std::string &filename) : Config(), _path(filename){
	std::ifstream		conf_stream;
	std::stringstream	ss;
	std::string			raw;
	
	if (this->_open_file(filename, conf_stream) == false)
	{
		throw std::runtime_error("[WebserverConfig] Unable to open path " + filename);
	}
	this->_cache_stream(conf_stream, ss);
	conf_stream.close();
	raw = this->_parse_readable_lines(ss);
	this->_directives = Config::parse_all_directives(raw, WebserverConfig::all_directives_set);
	this->_parse_server_conf(raw);
}

WebserverConfig &WebserverConfig::operator=(const WebserverConfig &ref){
	if (this != &ref)
	{
		this->_directives = ref._directives;
		this->_path = ref._path;
		this->_servers = ref._servers;
		this->_port_server_config_index_map = ref._port_server_config_index_map;
	}
	return *this;
}

bool	WebserverConfig::_open_file(const std::string &filename, std::ifstream& file){
	if (filename.empty() == true)
	{
		return false;
	}
	file.open(filename.c_str());
	return file.is_open();
}

void WebserverConfig::_cache_stream(std::ifstream& file, std::stringstream &cache_stream){
	cache_stream << file.rdbuf();
}

std::string WebserverConfig::_parse_readable_lines(std::stringstream &cached_stream){
	std::string	result;
	std::string line;
	std::size_t hash_pos;

	while (std::getline(cached_stream, line)) {
		hash_pos = line.find("#");
        if (line.find("#") != std::string::npos){
			line.erase(hash_pos);
		}
		if (line.find_first_not_of(" \t\r") == std::string::npos)
			continue;
        result += line + '\n';
    }
	return result;
}

void WebserverConfig::_parse_server_conf(const std::string &conf_str){
	std::multimap<std::string, std::string>	block_directives;
	std::pair<long, ServerConfig>		block_pair;

	block_directives = Config::parse_block_directives(conf_str);
	for (std::multimap<std::string, std::string>::iterator it = block_directives.begin(); it != block_directives.end(); ++it){
		if (it->first == "server"){
			block_pair.first = this->_servers.size();
			block_pair.second = ServerConfig(this->_directives, it->second);
			this->_servers.insert(block_pair);
			for (std::vector<unsigned int>::const_iterator cit = block_pair.second.ports().begin(); cit != block_pair.second.ports().end(); cit++)
			{
				this->_port_server_config_index_map[*cit].push_back(block_pair.first);
			}
		}
		else
			throw std::runtime_error("[WebserverConfig] Invalid block directive for this context");
	}
	
}

const std::string &WebserverConfig::path(void) const
{
	return this->_path;
}

const std::map<long, ServerConfig> WebserverConfig::servers() const
{
	return this->_servers;
}

std::map<unsigned int, std::vector<long> > WebserverConfig::get_port_server_config_index_map() const
{
	return this->_port_server_config_index_map;
}

}
