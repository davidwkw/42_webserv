#pragma once

#include <vector>
#include <map>
#include <string>
#include <set>

#include "../../includes/macros.hpp"
#include "../utils/utils.hpp"

namespace ft
{

class Config
{
	public:
		Config();
		virtual ~Config();
		Config(const Config &ref);
		Config &operator=(const Config &ref);
		Config(const std::map<std::string, std::string> &directives);
		Config(const std::map<std::string, std::string> &directives, std::set<std::string> inclusion_set);
		
		std::map<std::string, std::string>	directives() const;
		void								add_directive(std::pair<std::string, std::string> directive);
		
		static std::pair<std::string, std::string> parse_directive(const std::string &directive);

		std::map<std::string, std::string> parse_all_directives(const std::string &str, const std::set<std::string> &inclusion_set = Config::all_directives_set);

		std::multimap<std::string, std::string> parse_block_directives(const std::string &string);

	protected:		
		static const char 								*all_directives_array[];
		static const char 								*normal_directives_array[];
		static const char								*array_directives_array[];
		static const char 								*block_directives_array[];
		static const std::set<std::string> 				all_directives_set;
		static const std::set<std::string> 				normal_directives_set;
		static const std::set<std::string> 				array_directives_set;
		static const std::set<std::string>				block_directives_set;
		static const std::map<std::string, std::string>	directive_defaults;
		
		static void fill_directives(std::map<std::string, std::string> &directive_ref, const std::set<std::string> &inclusion_set, const std::map<std::string, std::string> &directive_map);

		std::vector<std::string>				find_normal_directive(const std::string &directive_key) const;
		std::vector<std::vector<std::string> >	find_array_directive(const std::string &directive_key) const;

		std::map<std::string, std::string>		_directives;

	private:
		static std::map<std::string, std::string> _init_directive_defaults();

};

}
