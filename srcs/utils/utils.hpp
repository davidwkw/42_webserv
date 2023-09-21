#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <exception>
#include <fstream>
#include <ios>
#include <vector>
#include <set>
#include <sstream>
#include <map>

#include "../../includes/macros.hpp"

namespace ft
{

std::string							ltrim_chars(const std::string& str, const std::string &characters);
std::string							rtrim_chars(const std::string& str, const std::string &characters);
std::string 						ret_str_perror(const std::string &msg);
std::string 						trim_chars(const std::string &str, const std::string &chars);
std::string 						str_limit_span(const std::string &str, const char open, const char close);
std::string							extract_between_boundaries(const std::string &text, const std::string &start_boundary, const std::string &end_boundary);
std::vector<std::string>			tokenise_str(const std::string & str, char c = ' ');
std::pair<std::string, std::string>	extract_key_value_pair(const std::string &str, const har delimiter_key);
unsigned long						hex_str_to_ulong(const std::string &hex_str);
bool								getline_CRLF(std::istream& input, std::string& line);
std::string							url_decode(const std::string &encoded_url);
std::string							string_vector_to_string(const std::vector<std::string> &vect);
std::string							prune_http_protocol(const std::string &domain_str);
std::size_t							calc_input_stream_size(std::istream &stream);
std::string							extract_file_extension(const std::string &filename);
std::string							str_to_uppercase(std::string str);
void								str_replace_char(std::string &str, char old_char, char new_char);
void								close_fd_helper(int fd);
std::map<std::string, std::string>	parse_key_value_string(const std::string &kv_string, const char kv_seperator , const char kv_delim);

template <typename T1, typename T2>
bool check_empty_pair(const std::pair<T1, T2> &pair_to_be_checked)
{
	return pair_to_be_checked.first == T1() && pair_to_be_checked.second == T2();
}

template <typename T>
std::string to_string(T val_to_convert)
{
	std::ostringstream oss;

	oss << val_to_convert;
	return oss.str();
}

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
