#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#include <fstream>
#include <iostream>
#include <exception>
#include <vector>
#include <set>
#include "Server.hpp"
#include <webserv.hpp>
#include <sstream>
#include <stdexcept>

class Config
{
	private:
		// Server stuff
		// listen
		// server_name
		// error_page
		// client_max_body_size

		static const std::set<std::string>	_valid_directives; 
		std::string							_path;
		std::map<std::string, Server>		_servers;
		std::map<std::string, std::string>	_directives;

		static std::set<std::string> _fill_valid_directives();
		Config(void);
		bool _open_file(const std::string &filename, std::ifstream& file);
		void _cache_stream(std::ifstream& file, std::stringstream &cached_stream);
		std::string _parse_readable_lines(std::stringstream &cached_stream);
		void _parse_server_conf(const std::string &cached_string);

	public:
		~Config(void);
		Config(const Config &ref);
		Config &operator=(const Config &ref);
		Config(const std::string &filename);

		const std::string &path(void) const;

		static std::pair<std::string, std::string> parse_simple_directive(const std::string &directive);
};

#endif