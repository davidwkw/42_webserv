#include "LimitExcept.hpp"

const char *LimitExcept::all_directives_array[] =	{
													"allow",
													"deny"
													};

const char *LimitExcept::normal_directives_array[] = 	{
														""
														};

const char *LimitExcept::array_directives_array[] =	{
													"allow",
													"deny"
													};
									
const char *LimitExcept::block_directives_array[] = 	{
														""
														};

const std::set<std::string> LimitExcept::all_directives_set = init_string_set(LimitExcept::all_directives_array);

const std::set<std::string> LimitExcept::normal_directives_set = init_string_set(LimitExcept::normal_directives_array);

const std::set<std::string> LimitExcept::array_directives_set = init_string_set(LimitExcept::array_directives_array);

const std::set<std::string> LimitExcept::block_directives_set = init_string_set(LimitExcept::block_directives_array);

LimitExcept::LimitExcept() : Config(){}

LimitExcept::~LimitExcept(){}

LimitExcept::LimitExcept(const LimitExcept &ref) : Config(ref){
	*this = ref;
}

LimitExcept &LimitExcept::operator=(const LimitExcept &ref){
	if (this != &ref)
	{
		this->_directives = ref._directives;
	}
	return *this;
}

LimitExcept::LimitExcept(Config::directive_container_type directives, const std::string &str) : Config(directives, LimitExcept::all_directives_set) {
	this->_directives = Config::parse_all_directives(location_str, LimitExcept::all_directives_set);
}

const std::vector<std::string> &LimitExcept::allow() const{
	std::vector<std::vector<std::string> > temp;
	std::vector<std::string> ret_vector;
	
	temp = this->find_array_directive("allow");
	for (std::vector<std::vector<std::string> >::iterator it = temp.begin(); it != temp.end(); ++it){
		for (std::vector<std::string>::iterator it2 = it->begin(); it2 != it->end(); ++it2){
			ret_vector.push_back(*it2);
		}
	}
	return ret_vector;
}

const std::vector<std::string> &LimitExcept::deny() const{
	std::vector<std::vector<std::string> > temp;
	std::vector<std::string> ret_vector;
	
	temp = this->find_array_directive("deny");
	for (std::vector<std::vector<std::string> >::iterator it = temp.begin(); it != temp.end(); ++it){
		for (std::vector<std::string>::iterator it2 = it->begin(); it2 != it->end(); ++it2){
			ret_vector.push_back(*it2);
		}
	}
	return ret_vector;
}
