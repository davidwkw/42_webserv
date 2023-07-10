#ifndef __RESPONSE__
#define __RESPONSE__

#include "../../includes/webserv.hpp"
#include <string>

namespace ft 
{

class Response
{
	public:
		enum ResponseWriteState
		{
			WRITING,
			FINISHED
		};

	private:
		float								_http_protocol;
		int									_status_code;
		std::map<std::string, std::string>	_headers;
		std::ifstream						_file_stream;
		std::string							_body;
		ResponseWriteState					_write_state;

		static const std::map<int, std::string>			_fill_reason_phrase_map();
		static const std::map<std::string, std::string>	_fill_mime_type_map();

	public:
		static const std::map<int, std::string>			reason_phrase_map;
		static const std::map<std::string, std::string> mime_type_map;

		Response(const float &protocol, int status_code = 0);
		Response(const Response &src);
		Response &operator=(const Response &src);
		~Response();

		std::stringstream	format_response() const;

		int					get_status_code() const;
		std::string			get_body() const;
		std::string 		get_string() const;
		ResponseWriteState	get_write_state() const;

		void 				set_status_code(const int &code);
		void 				set_body(const std::string &str);
};

}

#endif
