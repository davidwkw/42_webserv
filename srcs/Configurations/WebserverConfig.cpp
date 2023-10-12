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


WebserverConfig::~WebserverConfig(void){}

WebserverConfig::WebserverConfig(const WebserverConfig &ref) : Config(), _path(), _config_str(), _servers(), _port_server_config_index_map()
{
	*this = ref;
}

WebserverConfig::WebserverConfig(const std::string &filename) : Config(), _path(filename), _config_str()
{
	std::stringstream	ss;

	this->_cache_stream(filename, ss);
	this->_config_str = this->_parse_readable_lines(ss);
	this->_directives = Config::parse_all_directives(this->_config_str, WebserverConfig::all_directives_set);
	this->_parse_server_conf(this->_config_str);
	Config::fill_directives(this->_directives, WebserverConfig::all_directives_set, Config::directive_defaults);
}

WebserverConfig &WebserverConfig::operator=(const WebserverConfig &ref)
{
	if (this != &ref)
	{
		this->_directives = ref._directives;
		this->_path = ref._path;
		this->_config_str = ref._config_str;
		this->_servers = ref._servers;
		this->_port_server_config_index_map = ref._port_server_config_index_map;
	}
	return *this;
}

bool	WebserverConfig::_open_file(const std::string &filename, std::ifstream& file)
{
	if (filename.empty() == true)
	{
		return false;
	}
	file.open(filename.c_str());
	return file.is_open();
}

void WebserverConfig::_cache_stream(const std::string &filename, std::stringstream &cache_stream)
{
	std::ifstream	file_stream;
	std::string		line;

	if (this->_open_file(filename, file_stream) == false)
	{
		throw std::runtime_error("[WebserverConfig] Unable to open path " + filename);
	}
	while (std::getline(file_stream, line))
	{
        cache_stream << line << std::endl;
    }
}

std::string WebserverConfig::_parse_readable_lines(std::stringstream &cached_stream)
{
	std::string	result;
	std::string line;
	std::size_t hash_pos;

	while (std::getline(cached_stream, line)) 
	{
		hash_pos = line.find_first_of("#");
        if (hash_pos != std::string::npos)
		{
			line.erase(hash_pos);
		}
		line = trim_chars(line, WHITESPACE_CHARACTERS);
		if (line.empty())
		{
			continue;
		}
        result += line + '\n';
    }
	return result;
}

void WebserverConfig::_parse_server_conf(const std::string &conf_str)
{
	std::multimap<std::string, std::string>	block_directives;
	std::pair<long, ServerConfig>			block_pair;

	block_directives = Config::parse_block_directives(conf_str);
	for (std::multimap<std::string, std::string>::iterator it = block_directives.begin(); it != block_directives.end(); ++it)
	{
		if (it->first == "server")
		{
			std::string					&server_str = it->second;
			std::vector<unsigned int>	port_vector;

			block_pair.first = this->_servers.size();
			block_pair.second = ServerConfig(this->_directives, server_str);
			this->_servers.insert(block_pair);
			port_vector = block_pair.second.ports();
			for (std::vector<unsigned int>::iterator it = port_vector.begin(); it != port_vector.end(); it++)
			{
				this->_port_server_config_index_map[*it].push_back(block_pair.first);
			}
		}
		else
		{
			throw std::runtime_error("[WebserverConfig] Invalid block directive for this context");
		}
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

std::string WebserverConfig::config_string() const
{
	return this->_config_str;
}

}
