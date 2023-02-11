#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

#include <string>
#include <fstream>
#include <iostream>
#include <type_traits>

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

		void warn(const std::string &msg) const;
		void error(const std::string &msg) const;
		void info(const std::string &msg) const;
		void debug(const std::string &msg) const;

		bool isDefaultStream() const;
};

#endif
