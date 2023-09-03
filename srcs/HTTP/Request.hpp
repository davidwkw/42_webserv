#ifndef __REQUEST__
#define __REQUEST__

#include <string>
#include <map>
#include <cstdlib>
#include <utility>
#include "../../includes/webserv.hpp"

namespace ft
{

struct RequestLine
{
	std::string							method;
	std::string							target;
	std::string							uri;
	std::string							protocol;
	std::map<std::string, std::string>	query;

	RequestLine() : method(), uri(), protocol(), target(){}
	~RequestLine(){}
	RequestLine(const RequestLine& ref) : method(ref.method), uri(ref.uri), protocol(ref.protocol), target(ref.target), query(ref.query)
	{ 
		*this = ref;
	}
	RequestLine &operator=(const RequestLine& ref)
	{
		if (this != &ref)
		{
			this->method = ref.method;
			this->target = ref.target;
			this->uri = ref.uri;
			this->protocol = ref.protocol;
			this->query = ref.query;
		}
		return *this;
	}
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
		Request(const std::string &request_str);
		Request(const Request &ref);
		Request &operator=(const Request &ref);
		~Request();

#pragma region Getters

		std::string 						get_header(const std::string &key) const;
		std::string 						get_query_param(const std::string key) const;
		RequestReadState					get_request_read_state() const;
		std::string 						get_body_string() const;
		std::size_t							get_body_size() const;
		std::string 						get_protocol() const;
		std::map<std::string, std::string>	get_headers() const;
		std::string 						get_method() const;
		std::string							get_target() const;
		std::string 						get_uri() const;
		std::stringstream const &			get_body_stream() const;
		std::stringstream &					get_body_stream();

#pragma endregion Getters

#pragma region Setters

		void set_request_read_state(RequestReadState state);

#pragma endregion Setters

#pragma region PublicMemberMethods

		void append_to_request(const char *string);
		void process_request();

#pragma endregion PublicMemberMethods

	private:
		std::map<std::string, std::string>	_headers;
		std::stringstream					_buffer_stream;
		RequestLine							_request_start_line;
		std::stringstream					_body;
		RequestReadState					_read_state;

		std::map<std::string, std::string>	_extract_query(const std::string &uri_string);
		std::string							_extract_uri(const std::string &uri_string);
		std::string							_extract_target(const std::string &uri_string);
		std::string							_validate_method(std::string method);
		void 								_parse_request_line();
		void 								_validate_header_field(const std::pair<std::string, std::string> &header_pair);
		void 								_handle_duplicate_headers(const std::string &key, std::string &original_value, const std::string &duplicate_value);
		void 								_parse_request();
		void 								_parse_request_headers();
		void 								_parse_chunked_request_body();
		void 								_parse_encoded_request_body();

};

}

#endif
