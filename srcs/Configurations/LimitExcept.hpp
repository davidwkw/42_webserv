#ifndef __LIMIT_EXCEPT_HPP__
#define __LIMIT_EXCEPT_HPP__

#include <string>
#include <vector>
#include "BaseConfig.hpp"
#include "../utils/utils.hpp"

namespace ft
{	

class LimitExcept : public BaseConfig{
	private:
		static const char 								*all_directives_array[];
		static const char 								*normal_directives_array[];
		static const char								*array_directives_array[];
		static const char 								*block_directives_array[];
		static const std::set<std::string> 				all_directives_set;
		static const std::set<std::string> 				normal_directives_set;
		static const std::set<std::string> 				array_directives_set;
		static const std::set<std::string>				block_directives_set;

	public:	
		LimitExcept();
		~LimitExcept();
		LimitExcept(BaseConfig::directive_container_type directives, const std::string &str);
		LimitExcept(const LimitExcept &ref);
		LimitExcept &operator=(const LimitExcept &ref);

		const std::vector<std::string> &allow() const;
		const std::vector<std::string> &deny() const;
};
}

#endif
