#include "BaseConfig.hpp"

BaseConfig::BaseConfig() : _directives(){

}

BaseConfig::~BaseConfig(){

}

BaseConfig::BaseConfig(const BaseConfig &ref) : _directives(ref._directives){
	*this = ref;
}

BaseConfig &BaseConfig::operator=(const BaseConfig &ref){
	if (this != &ref)
	{
		this->_directives = ref._directives;
	}
	return *this;
}

BaseConfig::BaseConfig(const std::map<std::string,std::string> &directives){
	this->_directives = directives;
}

std::map<std::string, std::string> BaseConfig::directives() const{
	return this->_directives;
}

void BaseConfig::add_directive(std::pair<std::string, std::string> directive){
	this->_directives.insert(directive);
}

std::pair<std::string, std::string> BaseConfig::parse_simple_directive(const std::string &directive){
	std::stringstream temp(directive);
	std::string k;
	std::string v;
	std::string word;

	temp >> k;
	v = trim_ws_str(temp.str());
	
	return std::make_pair(k, v);
}
