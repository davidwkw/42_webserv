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
	this->_init(filename);
}

void	Config::_init(const std::string &filename){
	std::ifstream	config_stream;
	std::string 	line;

	// Might want to shift file handling to it's own class either inheriting fstream or using it as a variable
	if (filename.empty() == true){
		std::cerr << "Filename is empty" << std::endl;

	}

	config_stream.open(filename.c_str());

	if (config_stream.is_open() == false){
		std::cerr << "File could not open" << std::endl;
	}
	
	for (std::string line; std::getline(config_stream, line); ){
		
	}

}

const std::string Config::path(void) const{
	return this->_path;
}

bool Config::_is_valid_directive(const std::string &directive) const{
	std::set<std::string> directives = {
										"Hello",
										"Goodbye",
										"Good morning"
										};
}


const char* Config::ConfigurationFailed::what() const throw()
{
	return ("Failed to configure object");
}
