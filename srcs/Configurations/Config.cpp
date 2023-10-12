#include "Config.hpp"

namespace ft
{

const char *Config::all_directives_array[] = {
												"error_log",
												"server_name",
												"listen",
												"index",
												"error_page",
												"redirect",
												"cgi_bin",
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
									
const char *Config::normal_directives_array[] = {
													"index",
													"redirect",
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

const char *Config::array_directives_array[] =	{
													"error_page",
													"error_log",
													"server_name",
													"listen",
													"allow",
													"cgi_bin",
													"deny"
												};

const char *Config::block_directives_array[] =	{
													"server",
													"location",
												};

const std::set<std::string> Config::all_directives_set = init_string_set(Config::all_directives_array);

const std::set<std::string> Config::normal_directives_set = init_string_set(Config::normal_directives_array);

const std::set<std::string> Config::array_directives_set = init_string_set(Config::array_directives_array);

const std::set<std::string> Config::block_directives_set = init_string_set(Config::block_directives_array);

const std::map<std::string, std::string> Config::directive_defaults = Config::_init_directive_defaults();

std::map<std::string, std::string> Config::_init_directive_defaults(){

	std::map<std::string, std::string> fill_map;

	fill_map["server_name"] = "";
	fill_map["error_log"] = "logs/error.log error";
	fill_map["index"] = "index.html";
	fill_map["listen"] = "8000";
	fill_map["user"] = "nobody nobody";
	fill_map["pid"] = "logs/nginx.pid";
	fill_map["worker_processes"] = "1";
	fill_map["autoindex"] = "off";
	fill_map["root"] = "public";
	fill_map["client_body_temp_path"] = "client_body_temp";
	fill_map["client_max_body_size"] = "1m";
	fill_map["limit_except"] = "GET";
	
	return fill_map;
}

Config::Config() : _directives(){}

Config::~Config(){}

Config::Config(const Config &ref) : _directives(ref._directives)
{
	*this = ref;
}

Config &Config::operator=(const Config &ref)
{
	if (this != &ref)
	{
		this->_directives = ref._directives;
	}
	return *this;
}

Config::Config(const std::map<std::string, std::string> &directives) : _directives()
{
	this->_directives = directives;
}

Config::Config(const std::map<std::string, std::string> &directives, std::set<std::string> inclusion_set) : _directives()
{
	std::map<std::string, std::string>::const_iterator cit;
	std::pair<std::string, std::string> temp;

	for (std::set<std::string>::iterator it = inclusion_set.begin(); it != inclusion_set.end(); ++it)
	{
		cit = directives.find(*it);
		if (cit != directives.end())
			this->_directives.insert(*cit);
	}
}

std::map<std::string, std::string> Config::directives() const
{
	return this->_directives;
}

void Config::add_directive(std::pair<std::string, std::string> directive)
{
	this->_directives.insert(directive);
}

std::pair<std::string, std::string> Config::parse_directive(const std::string &directive)
{
	std::stringstream 				temp;
	std::string 					word;
	std::string 					k;
	std::string						v;

	temp.str(trim_chars(directive, WHITESPACE_CHARACTERS));
	temp >> k;
	while (temp >> word)
	{
		v += (v.empty() ? "" :  " " ) + word;
	}
	
	return std::pair<std::string, std::string>(k, v);
}

std::map<std::string, std::string> Config::parse_all_directives(const std::string &str, const std::set<std::string> &inclusion_set)
{
	std::map<std::string, std::string> container;
	std::size_t start_index = 0;
	std::size_t delimiter_index = start_index;
	std::string captured_directive;
	std::pair<std::string, std::string> directive_pair;

  	while ((delimiter_index = str.find_first_of(";{}", delimiter_index)) != std::string::npos)
	{
		captured_directive = str.substr(start_index, delimiter_index - start_index);
		if (str[delimiter_index] == ';')
		{
			directive_pair = Config::parse_directive(trim_chars(captured_directive, WHITESPACE_CHARACTERS));
		}
		else if (str[delimiter_index] == '{')
		{
			captured_directive = str_limit_span(str.c_str() + delimiter_index, '{', '}');
			if (captured_directive.empty())
			{
				throw std::runtime_error("[Config] Missing } found in configuration file");
			}
			delimiter_index += captured_directive.length() + 2;
			continue;
		}
		else if (str[delimiter_index] == '}')
		{
			throw std::runtime_error("[Config] Found } without { in configuration file");
		}
		if (inclusion_set.find(directive_pair.first) == inclusion_set.end())
		{
			throw std::runtime_error("[Config] Invalid directive " + directive_pair.first);
		}
		if (Config::normal_directives_set.find(directive_pair.first) != Config::normal_directives_set.end()
			&& this->_directives.find(directive_pair.first) != this->_directives.end())
		{
			throw std::runtime_error("[Config] " + directive_pair.first + " directive already set");
		}
		else if (Config::array_directives_set.find(directive_pair.first) != Config::array_directives_set.end())
		{
			std::string delim;
		
			try
			{
				container.at(directive_pair.first);
				delim = ";";
			}
			catch (const std::out_of_range &e)
			{
				delim = "";
			}

			container[directive_pair.first] += delim + directive_pair.second;
		}
		else
		{
			container.insert(directive_pair);
		}
		++delimiter_index;
		start_index = delimiter_index;
	}
	return container;
}

void Config::fill_directives(std::map<std::string, std::string> &directive_ref, const std::set<std::string> &inclusion_set, const std::map<std::string, std::string> &directive_map)
{
	for (std::set<std::string>::const_iterator cit = inclusion_set.begin(); cit != inclusion_set.end(); ++cit)
	{
		std::map<std::string, std::string>::const_iterator dit = directive_map.find(*cit);
		if (dit != directive_map.end())
		{
			directive_ref.insert(*dit);
		}
	}
}

std::multimap<std::string, std::string> Config::parse_block_directives(const std::string &str)
{
	std::multimap<std::string, std::string>	result;
	std::pair<std::string, std::string>		directive_pair;
	std::size_t								start_index = 0;
	std::size_t								delimiter_index = start_index;
	
	while ((delimiter_index = str.find_first_of("{}", delimiter_index)) != std::string::npos)
	{
		if (str[delimiter_index] == '}')
		{
			throw std::runtime_error("[Config]: Found } before {");
		}
		start_index = str.find_last_of("};", delimiter_index);
		start_index %= std::string::npos;
		start_index = (start_index > 0) ? start_index + 1 : start_index;
		directive_pair.first = str.substr(start_index, delimiter_index - start_index);
		directive_pair.first = trim_chars(directive_pair.first, WHITESPACE_CHARACTERS);
		directive_pair.second = str_limit_span(str.c_str() + delimiter_index, '{', '}');
		delimiter_index += directive_pair.second.size() + 2;
		directive_pair.second = trim_chars(directive_pair.second, WHITESPACE_CHARACTERS);
		result.insert(directive_pair);
	}
	return result;
}

std::vector<std::string> Config::find_normal_directive(const std::string &directive_key) const
{
	std::map<std::string, std::string>::const_iterator cit;
	std::vector<std::string> ret_vector;

	cit = this->_directives.find(directive_key);
	if (cit != this->_directives.end())
	{
		ret_vector = tokenise_str(cit->second);
	}
	return ret_vector;
}

std::vector<std::vector<std::string > > Config::find_array_directive(const std::string &directive_key) const
{
	std::map<std::string, std::string>::const_iterator	cit;
	std::vector<std::vector<std::string> >				ret_vector;

	cit = this->_directives.find(directive_key);
	if (cit != this->_directives.end())
	{
		std::vector<std::string> directive_vect;

		directive_vect = tokenise_str(cit->second, ';');
		for (std::vector<std::string>::iterator it = directive_vect.begin(); it != directive_vect.end(); it++)
		{
			ret_vector.push_back(tokenise_str(*it, ' '));
		}
	}
	return ret_vector;
}
	
}
