#pragma once

#include <string>
#include <map>
#include <cstdlib>
#include <utility>
#include <algorithm>
#include "../../includes/macros.hpp" 
#include "../utils/utils.hpp"
#include "../Exception/HTTPException.hpp"

namespace ft
{

struct RequestLine
{
	std::string							method; 
	std::string							target;
	std::string							uri; 
	std::string							protocol;
	std::string							target_file; 
	std::map<std::string, std::string>	query;

	RequestLine();
	~RequestLine();
	RequestLine(const RequestLine& ref);
	RequestLine &operator=(const RequestLine& ref);
};

class Request
{
	public:
		enum RequestReadState
		{
			READING,
			FINISHED
		};

		Request();
		~Request();

		std::string 						get_header(const std::string &key) const;
		std::string 						get_query_param(const std::string key) const;
		RequestReadState					get_request_read_state() const;
		std::string 						get_body_string() const;
		std::size_t							get_body_size() const;
		std::string 						get_protocol() const;
		std::map<std::string, std::string>	get_headers() const;
		std::string 						get_method() const;
		std::string							get_target() const;
		std::string							get_target_file() const;
		std::string 						get_uri() const;
		std::string							get_query_string() const;
		std::stringstream const &			get_body_stream() const;
		std::stringstream &					get_body_stream();

		void set_request_read_state(RequestReadState state);

		void append_to_request(std::string str);
		void process_request();

	private:
		RequestReadState					_read_state;
		RequestLine							_request_start_line;
		std::map<std::string, std::string>	_headers;
		std::stringstream					_buffer_stream;
		std::stringstream					_body;
		std::size_t							_header_break_index;

		std::string							_extract_target_file(const std::string &uri_string);
		std::map<std::string, std::string>	_extract_query(const std::string &uri_string);
		std::string							_extract_uri(const std::string &uri_string);
		std::string							_extract_target(const std::string &uri_string);
		std::string							_validate_method(const std::string &method);
		void 								_parse_request_line();
		void 								_validate_header_field(const std::pair<std::string, std::string> &header_pair);
		void 								_handle_duplicate_headers(const std::string &key, std::string &original_value, const std::string &duplicate_value);
		void 								_parse_request();
		void 								_parse_request_headers();
		void 								_parse_chunked_request_body();
		void 								_parse_encoded_request_body();
};

}
