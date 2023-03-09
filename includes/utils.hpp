#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <string>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <exception>
#include <fstream>
#include <ios>
#include <vector>

std::string ret_str_error(const std::string &msg);
std::string trim_str(const std::string &str, char *chars);
std::string str_char_limit_span(const std::string &str, char open, char close);
std::vector<std::string> tokenise_str(const std::string & str, char c = ' ');

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

#endif
