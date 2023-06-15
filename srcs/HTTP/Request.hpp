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
	std::string method;
	std::string uri;
	std::string protocol;
	std::map<std::string, std::string> query;

	RequestLine() : method(), uri(), protocol(){}
	RequestLine(const RequestLine& ref) : method(ref.method), uri(ref.uri), protocol(ref.protocol)
	{ 
		*this = ref;
	}
	RequestLine &operator=(const RequestLine& ref)
	{
		if (this != &ref)
		{
			this->method = ref.method;
			this->uri = ref.uri;
			this->protocol = ref.protocol;
		}
		return *this;
	}
};

class Request
{
	private:
		RequestLine _request_start_line;
		std::map<std::string, std::string> _headers;
		std::string _body;

		std::string _extract_uri(const std::string &uri_string);
		std::map<std::string, std::string> _extract_query(const std::string &uri_string);
		std::string _validate_method(std::string method);
		void _parse_request_line(std::istream& iss);
		// void _validate_request_start_line_break(const std::string &headers);
		void _validate_header_field(const std::pair<std::string, std::string> &header_pair);
		void _handle_duplicate_headers(const std::string &key, std::string &original_value, const std::string &duplicate_value);
		void _parse_request_string(const std::string &request_string);
		void _parse_request_headers(std::istream& iss);
		void _parse_chunked_request_body(std::istream& iss);
		void _parse_encoded_request_body(std::istream& iss);
		
	public:
		Request();
		Request(const std::string &request_str);
		Request(const Request &src);
		Request &operator=(const Request &src);
		Request &operator=(const std::string &request_str);
		~Request();

#pragma region Getters

		std::string get_method() const;
		std::string get_uri() const;
		std::string get_protocol() const;
		std::string get_header(const std::string &key) const;
		std::string get_body() const;
		std::string get_query_param(const std::string key) const;

#pragma endregion Getters

};

}

#endif
