#pragma once

#include <string>
#include <map>
#include <cstdlib>
#include <utility>
#include <algorithm>
#include <sstream>
#include <memory>
#include <algorithm>
#include <stdexcept>

#include "../../includes/macros.hpp" 
#include "../utils/utils.hpp"
#include "../Exception/HTTPException.hpp"
#include "RequestLine.hpp"
#include "RequestBodyFactory.hpp"
#include "RequestBody.hpp"

namespace ft
{

class Request
{
	public:
		Request();
		~Request();

		void								construct_message_format(const std::string &message_format_string);

		std::string 						get_header(const std::string &key) const;
		std::string 						get_query_param(const std::string key) const;
		std::string 						get_body_string() const;
		std::size_t							get_body_size() const;
		std::string 						get_protocol() const;
		std::map<std::string, std::string>	get_headers() const;
		std::string 						get_method() const;
		std::string							get_target() const;
		std::string							get_target_file() const;
		std::string							get_query_string() const;
		std::stringstream &					get_raw_body_stream();
		std::string							get_raw_body_string() const;
		RequestBody							&get_body() const;

		void								set_body(RequestBody *request_body_ptr);

		bool								has_body_headers() const;

	private:
		RequestLine							_request_start_line;
		std::map<std::string, std::string>	_headers;
		std::stringstream					_buffer_stream;
		std::auto_ptr<RequestBody>			_body;
		std::size_t							_header_break_index;

		std::string							_extract_target_file(const std::string &uri_string);
		std::map<std::string, std::string>	_extract_query(const std::string &uri_string);
		std::string							_extract_uri(const std::string &uri_string);
		std::string							_extract_target(const std::string &uri_string);
		std::string							_identify_content_type();
		std::string							_identify_boundary_string();
		std::string							_validate_method(const std::string &method);
		void 								_validate_header_field(const std::pair<std::string, std::string> &header_pair);
		void 								_handle_duplicate_headers(const std::string &key, const std::string &original_value, const std::string &duplicate_value);
		void 								_parse_request();
		std::string 						_parse_chunked_request_body();
		std::string 						_parse_encoded_request_body();
		void								_parse_request_line(std::stringstream &ss);
		void								_parse_request_headers(std::stringstream &ss);



};

}
