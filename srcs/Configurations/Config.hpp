#ifndef __BASE_CONFIG_HPP__
#define __BASE_CONFIG_HPP__

#include <map>
#include <string>
#include <sstream>
#include <set>
#include <vector>
#include "webserv.hpp"

namespace ft
{
	
class Config{
	protected:
	
		typedef std::map<std::string, std::string>	directive_container_type;
		typedef std::pair<std::string, std::string> directive_type;
		typedef std::set<std::string>				allowed_directives_container_type;
		
		static const char 								*all_directives_array[];
		static const char 								*normal_directives_array[];
		static const char								*array_directives_array[];
		static const char 								*block_directives_array[];
		static const std::set<std::string> 				all_directives_set;
		static const std::set<std::string> 				normal_directives_set;
		static const std::set<std::string> 				array_directives_set;
		static const std::set<std::string>				block_directives_set;
		static const directive_container_type			directive_defaults;
		
		void _fill_directive_defaults(directive_container_type &directive_ref, const allowed_directives_container_type &inclusion_set, const directive_container_type &defaults_map = Config::directive_defaults);

		const std::vector<std::string> find_normal_directive(const std::string &directive_key) const;
		const std::vector<std::vector<std::string> > find_array_directive(const std::string &directive_key) const;

		directive_container_type _directives;

	private:
		static std::map<std::string, std::string> _init_directive_defaults();

	public:
		Config();
		virtual ~Config();
		Config(const Config &ref);
		Config &operator=(const Config &ref);
		Config(const directive_container_type &directives);
		Config(const directive_container_type &directives, allowed_directives_container_type inclusion_set);
		template <class Iterator>
		Config(const directive_container_type &directives, Iterator first, Iterator last,
					typename enable_if<is_same<typename remove_const<typename Iterator::value_type>::type, std::string>::value ||
								   is_same<typename remove_const<typename Iterator::value_type>::type, char*>::value>::type* = nullptr);
		
		directive_container_type directives() const;
		void add_directive(directive_type directive);
		
		static directive_type parse_directive(const std::string &directive);

		directive_container_type parse_all_directives(const std::string &str, const allowed_directives_container_type &inclusion_set = Config::all_directives_set);

		std::map<std::string, std::string> parse_block_directives(const std::string &string);
};

template <class Iterator>
Config::Config(const directive_container_type &directives, Iterator first, Iterator last, typename enable_if<is_same<typename remove_const<typename Iterator::value_type>::type, std::string>::value || is_same<typename remove_const<typename Iterator::value_type>::type, char*>::value>::type* = nullptr){
	Iterator first_copy = first;
	
	this->_directives = directives;
	for (typename directive_container_type::iterator directives_start = directives.begin(); directives_start != directives.end(); ++directives_start)
	{
		while (first != last)
		{
			if (*directives_start == *first)
				break;
			++first;
		}
		if (first == last)
			this->_directives.erase(*directives_start);
		first = first_copy;
	}
}

}

#endif
