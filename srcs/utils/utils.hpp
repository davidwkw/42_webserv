#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <exception>
#include <fstream>
#include <ios>
#include <vector>

namespace ft
{
std::string ret_str_error(const std::string &msg);
std::string trim_str(const std::string &str, char *chars);
std::string str_char_limit_span(const std::string &str, char open, char close);
std::vector<std::string> tokenise_str(const std::string & str, char c = ' ');
std::pair<std::string, std::string> extract_key_value_pair(const std::string &str, char delimiter_key);
unsigned long hex_str_to_ulong(const std::string &hex_str);
bool getline_CRLF(std::istream& input, std::string& line);
std::string url_decode(const std::string &encoded_url);

template<bool Cond, class T = void>
struct enable_if {};

template<class T>
struct enable_if<true, T> { typedef T type; };

// is_integral
template <class T, T v>
struct integral_constant {
	enum {value = v};
	typedef T value_type;
	typedef integral_constant<T,v> type;
	operator T() { return v; }
};

typedef integral_constant<bool,false> false_type;
typedef integral_constant<bool,true> true_type;

template<class T, class U>
struct is_same : std::false_type {};
 
template<class T>
struct is_same<T, T> : std::true_type {};

template< class T > struct remove_const                { typedef T type; };
template< class T > struct remove_const<const T>       { typedef T type; };

template <std::size_t N>
std::vector<std::string> init_string_vector(const char *(&arr)[N]){

	std::vector<std::string> fill_vector;

	for (std::size_t i = 0; i < N; i++)
		fill_vector.insert(arr[i]);
	return fill_vector;
}

template <std::size_t N>
std::set<std::string> init_string_set(const char *(&arr)[N]){

	std::set<std::string> fill_set;

	for (std::size_t i = 0; i < N; i++)
		fill_set.insert(arr[i]);
	return fill_set;
}

}
#endif
