#ifndef __BASE_CONFIG_HPP__
#define __BASE_CONFIG_HPP__

#include <map>
#include <string>
#include <sstream>
#include <set>
#include "webserv.hpp"

class BaseConfig{
	protected:
		enum Actions{
			APPEND_EXISTING,
			INSERT_NEW,
			OVERRIDE,
			DUPLICATE_ERROR
		};

		enum DirectiveStatus{
			DEFAULT,
			INHERITED,
			DEFINED
		};
	
		typedef std::map<std::string, std::pair<std::string, DirectiveStatus> > directive_container_type;
		typedef typename directive_container_type::value_type					directive_type;
		typedef typename directive_container_type::mapped_type					directive_value_type;
		
		static const char 								*simple_directive_array[];
		static const char								*array_directive_array[];
		static const char 								*block_directive_array[];
		static const std::set<std::string> 				_simple_directives_set;
		static const std::set<std::string> 				_array_directives_set;
		static const std::set<std::string>				_block_directives_set;
		static const std::map<std::string, Actions>		_directive_actions;
		static const std::map<std::string, std::string>	directive_defaults;

		static std::map<std::string, std::string> _init_directive_defaults();

		template <std::size_t N>
		static std::set<std::string> _init_directive_set(const char *(&arr)[N]);
		
		directive_container_type _directives;

	private:
		static std::map<std::string, Actions> _fill_directive_actions();

	public:
		BaseConfig();
		virtual ~BaseConfig();
		BaseConfig(const BaseConfig &ref);
		BaseConfig &operator=(const BaseConfig &ref);
		BaseConfig(const directive_container_type &directives);

		template <class Iterator>
		BaseConfig(const directive_container_type &directives, Iterator first, Iterator last,
				typename enable_if<is_same<typename remove_const<typename Iterator::value_type>::type, std::string>::value ||
								   is_same<typename remove_const<typename Iterator::value_type>::type, char*>::value>::type* = nullptr);
		
		directive_container_type directives() const;
		void add_directive(directive_type directive);
		
		static directive_type parse_simple_directive(const std::string &directive, BaseConfig::DirectiveStatus status = DEFINED);

		
};

template <class Iterator>
BaseConfig::BaseConfig(const directive_container_type &directives, Iterator first = BaseConfig::_valid_directives.begin(), Iterator last = BaseConfig::_valid_directives.begin(),
				typename enable_if<is_same<typename remove_const<typename Iterator::value_type>::type, std::string>::value
								|| is_same<typename remove_const<typename Iterator::value_type>::type, char*>::value>::type* = nullptr){
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

template <std::size_t N>
std::set<std::string> BaseConfig::_init_directive_set(const char *(&arr)[N]){

	std::set<std::string> fill_set;

	for (std::size_t i = 0; i < N; i++)
		fill_set.insert(arr[i]);
	return fill_set;
}

#endif
