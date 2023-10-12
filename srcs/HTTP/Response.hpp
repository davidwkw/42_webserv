#pragma once

#include <map>
#include <string>
#include <sstream>
#include <memory>
#include "../utils/utils.hpp"

namespace ft 
{

class Response
{
	public:
		static const std::map<int, std::string> _fill_reason_phrase_map();

		Response();
		Response(float protocol_version);
		~Response();

		void				construct_response_message_format();
		std::string			read_response(std::size_t buffer_size);
		void				unread_response(std::size_t	size);

		std::istream 		*get_message_format() const;
		std::istream const	*get_body_stream() const;
		std::istream 		*get_body_stream();
		int					get_status_code() const;
		bool				has_been_completely_read() const;

		void 				set_status_code(const int &code);
		void				set_body_stream(std::istream *body_stream);
		void				set_body_stream(std::auto_ptr<std::istream> body_stream);
		void				append_headers(const std::map<std::string, std::string> &header_map);
		void				set_header(const std::string &key, const std::string &value);
		void				remove_header(const std::string &key);
		
	private:
		static const std::map<int, std::string>	reason_phrase_map;
		
		float									_http_protocol;
		int										_status_code;
		std::map<std::string, std::string>		_headers;
		std::auto_ptr<std::stringstream>		_message_format;
		std::auto_ptr<std::istream>				_body_stream;

};

}
