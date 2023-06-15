#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

#include <string>
#include <fstream>
#include <iostream>
#include <type_traits>
#include "../../includes/colors.hpp"

namespace ft
{

class Logger{
	private:
		std::string		_log_path;
		std::ostream	*_log_stream;
		bool			_ext;
		bool			_is_default;

		Logger(const Logger &ref);
		Logger &operator=(const Logger &ref);
	public:
		explicit Logger(std::ostream *default_stream = &std::cout);
		~Logger();
		Logger(const std::string &path, std::ostream *default_stream = &std::cout);

		template < class T >
		void warn(const T &msg) const;

		template < class T >
		void error(const T &msg) const;

		template < class T >
		void info(const T &msg) const;

		template < class T >
		void debug(const T &msg) const;

		bool isDefaultStream() const;
};

template < class T >
void Logger::info(const T &msg) const {
	(*_log_stream) << BOLD << BLUE << "[Info] " << RESET << msg << std::endl;
}

template < class T >
void Logger::debug(const T &msg) const{
	(*_log_stream) << BOLD << GREEN << "[Debug] " << RESET << msg << std::endl;
}

template < class T >
void Logger::warn(const T &msg) const {
	(*_log_stream) << BOLD << YELLOW << "[Warning] " << RESET << msg << std::endl;
}

template < class T >
void Logger::error(const T &msg) const {
	if (_log_stream == &std::cout)
		std::cerr << BOLD << RED << "[Error] " << msg << std::endl;
	else
		(*_log_stream) << BOLD << RED << "[Error] " << RESET << msg << std::endl;
}

}

#endif
