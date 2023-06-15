#include "BaseConfig.hpp"

const char *ft::BaseConfig::all_directives_array[] = {
											"error_log",
											"server_name",
											"listen",
											"index",
											"error_page",
											"return",
											"cgi",
											"user",
											"pid",
											"worker_processes",
											"autoindex",
											"root",
											"client_body_temp_path",
											"try_files",
											"client_max_body_size"
											};
									
const char *ft::BaseConfig::normal_directives_array[] = {
													"index",
													"error_page",
													"return",
													"cgi",
													"user",
													"pid",
													"worker_processes",
													"autoindex",
													"root",
													"client_body_temp_path",
													"try_files",
													"client_max_body_size"
													};

const char *ft::BaseConfig::array_directives_array[] =	{
													"error_log",
													"server_name",
													"listen",
													"allow",
													"deny"
													};

const char *ft::BaseConfig::block_directives_array[] =	{
													"server",
													"location",
													"limit_except"
													};

const std::set<std::string> ft::BaseConfig::all_directives_set = init_string_set(ft::BaseConfig::all_directives_array);

const std::set<std::string> ft::BaseConfig::all_directives_set = init_string_set(ft::BaseConfig::normal_directives_array);

const std::set<std::string> ft::BaseConfig::array_directives_set = init_string_set(ft::BaseConfig::array_directives_array);

const std::set<std::string> ft::BaseConfig::block_directives_set = init_string_set(ft::BaseConfig::block_directives_array);

const std::map<std::string, std::string> ft::BaseConfig::directive_defaults = ft::BaseConfig::_init_directive_defaults();

std::map<std::string, std::string> ft::BaseConfig::_init_directive_defaults(){
	typedef std::pair<std::string, std::string> directive_default_pair;
	
	const directive_default_pair directives_default_pairs[]	=	{
												directive_default_pair("server_name", ""),
												directive_default_pair("error_log", "logs/error.log error"),
												directive_default_pair("index", "index.html"),
												directive_default_pair("listen", "8000"),
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
		fill_map.insert(directives_default_pairs[i]);
	return fill_map;
}

ft::BaseConfig::BaseConfig() : _directives(){
	_fill_directive_defaults(this->_directives, BaseConfig::all_directives_set);
}

ft::BaseConfig::~BaseConfig(){}

ft::BaseConfig::BaseConfig(const BaseConfig &ref) : _directives(ref._directives){
	*this = ref;
}

ft::BaseConfig &ft::BaseConfig::operator=(const BaseConfig &ref){
	if (this != &ref){
		this->_directives = ref._directives;
	}
	return *this;
}

ft::BaseConfig::BaseConfig(const directive_container_type &directives) : _directives(){
	this->_directives = directives;
	_fill_directive_defaults(this->_directives, BaseConfig::all_directives_set);
}

ft::BaseConfig::BaseConfig(const directive_container_type &directives, allowed_directives_container_type inclusion_set) : _directives() {
	
	directive_container_type::const_iterator cit;
	directive_type temp;

	for (allowed_directives_container_type::iterator it = inclusion_set.begin(); it != inclusion_set.end(); ++it)
	{
		cit = directives.find(*it);
		if (cit != directives.end())
			this->_directives.insert(*cit);
	}
	_fill_directive_defaults(this->_directives, inclusion_set);
}

ft::BaseConfig::directive_container_type ft::BaseConfig::directives() const{
	return this->_directives;
}

void ft::BaseConfig::add_directive(directive_type directive){
	this->_directives.insert(directive);
}

ft::BaseConfig::directive_type ft::BaseConfig::parse_directive(const std::string &directive){
	std::stringstream 				temp;
	std::string 					word;
	std::string 					k;
	std::string						v;

	temp.str(trim_str(directive, " \n\t"));
	temp >> k;
	while (temp >> word){
		v += (v.empty() ? "" :  " " ) + word;
	}
	
	return directive_type(k, v);
}

ft::BaseConfig::directive_container_type ft::BaseConfig::parse_all_directives(const std::string &str, const allowed_directives_container_type &inclusion_set = ft::BaseConfig::all_directives_set){
	BaseConfig::directive_container_type container;
	std::size_t start_index = 0;
	std::size_t delimiter_index = start_index;
	std::string captured_directive;
	BaseConfig::directive_type directive_pair;

  	while ((delimiter_index = str.find_first_of(";{}", delimiter_index)) != std::string::npos)
	{
		captured_directive = str.substr(start_index, delimiter_index - start_index);
		if (str[delimiter_index] == ';'){
			directive_pair = BaseConfig::parse_directive(trim_str(captured_directive, " \n\t"));
		}
		else if (str[delimiter_index] == '{')
		{
			captured_directive = str_char_limit_span(str.c_str() + delimiter_index, '{', '}');
			if (captured_directive.empty() == true)
				throw std::runtime_error("[BaseConfig] Missing } found in configuration file");
			delimiter_index += captured_directive.length() + 2;
			continue;
		}
		else
			throw std::runtime_error("[BaseConfig] Found } without { in configuration file");
		if (inclusion_set.find(directive_pair.first) == inclusion_set.end())
			throw std::runtime_error("[BaseConfig] Invalid directive " + directive_pair.first);
		if (BaseConfig::normal_directives_set.find(directive_pair.first) != BaseConfig::normal_directives_set.end()
			&& this->_directives.find(directive_pair.first) != this->_directives.end())
			throw std::runtime_error("[BaseConfig] " + directive_pair.first + " directive already set");
		else if (BaseConfig::array_directives_set.find(directive_pair.first) != BaseConfig::array_directives_set.end()
			&& this->_directives.find(directive_pair.first) != this->_directives.end())
			this->_directives[directive_pair.first] += ';' + directive_pair.second;
		else
			this->_directives.insert(directive_pair);
		++delimiter_index;
		start_index = delimiter_index;
	}
	return container;
}

void ft::BaseConfig::_fill_directive_defaults(directive_container_type &directive_ref, const allowed_directives_container_type &inclusion_set, const directive_container_type &defaults_map = ft::BaseConfig::directive_defaults){
	BaseConfig::directive_container_type::const_iterator dit;

	for (allowed_directives_container_type::const_iterator cit = inclusion_set.begin(); cit != inclusion_set.end(); ++cit){
		dit = defaults_map.find(*cit);
		if (dit != defaults_map.end())
			directive_ref.insert(*dit);
	}
}

std::map<std::string, std::string> ft::BaseConfig::parse_block_directives(const std::string &str){
	std::map<std::string, std::string> return_map;
	std::pair<std::string, std::string> directive_pair;
	std::size_t start_index = 0;
	std::size_t delimiter_index = start_index;
	
	while ((delimiter_index = str.find_first_of("{}", delimiter_index)) != std::string::npos){
		start_index = str.find_last_of(';', delimiter_index);
		if (start_index == std::string::npos)
			start_index = 0;
		directive_pair.first = str.substr(start_index, delimiter_index - start_index);
		directive_pair.first = trim_str(directive_pair.first, " \n\t");
		directive_pair.second = str_char_limit_span(str.c_str() + delimiter_index, '{', '}');
		return_map.insert(directive_pair);
		delimiter_index += directive_pair.second.size() + 2;
	}
	return return_map;
}

const std::vector<std::string> ft::BaseConfig::find_normal_directive(const std::string &directive_key) const {
	directive_container_type::const_iterator cit;
	std::vector<std::string> ret_vector;

	cit = this->_directives.find(directive_key);
	if (cit != this->_directives.end())
		ret_vector = tokenise_str(cit->second);
	return ret_vector;
}

const std::vector<std::vector<std::string > > ft::BaseConfig::find_array_directive(const std::string &directive_key) const{
	directive_container_type::const_iterator cit;
	std::vector<std::vector<std::string> > ret_vector;

	cit = this->_directives.find(directive_key);
	if (cit != this->_directives.end())
		ret_vector.push_back(tokenise_str(cit->second, ';'));
	return ret_vector;
}
