#include "RequestMultipart.hpp"

// note: multipart section MUST have header of "Content-Disposition: form-data; name=""" at minimum, filename optional

// if filename == "" use name ""

// multifile is sent in different parts sharing the same name

// only Content-Type and Content-Disposition header and possibly Content-Transfer-Encoding is parsed, ignore every other field

namespace ft
{

RequestMultipart::RequestMultipart() : _headers(), _content()
{
	this->_headers["Content-Type"] = "text/plain"; // default if no content-type was specified
}

RequestMultipart::RequestMultipart(const std::map<std::string, std::string> &headers, const std::string &content) : _headers(headers), _content(content){}

RequestMultipart::~RequestMultipart(){}

RequestMultipart::RequestMultipart(const RequestMultipart& ref) : _headers(ref._headers), _content(ref._content)
{
	*this = ref;
}

RequestMultipart & RequestMultipart::operator=(const RequestMultipart& ref)
{
	if (this != &ref)
	{
		this->_headers = ref._headers;
		this->_content = ref._content;
	}
	return *this;
}

std::string	RequestMultipart::get_filename() const
{
	return trim_chars(this->_extract_key_from_header("Content-Disposition", "filename"), "\"");
}

std::string RequestMultipart::get_name() const
{
	std::string name;

	name = this->_extract_key_from_header("Content-Disposition", "name");

	return name;
}

std::string RequestMultipart::get_content_type() const
{
	return this->_headers.at("Content-Type");
}

std::map<std::string, std::string> RequestMultipart::get_headers() const
{
	return this->_headers;
}

void RequestMultipart::set_headers(const std::map<std::string, std::string> &headers)
{
	this->_headers = headers;
}

void RequestMultipart::set_content(const std::string &content)
{
	this->_content = content;
}


std::string	RequestMultipart::_extract_key_from_header(const std::string &header_name, const std::string &key) const
{
	std::string							content_disposition;
	std::vector<std::string>			string_tokens;
	std::string							result;

	content_disposition = this->_headers.at(header_name);
	string_tokens = tokenise_str(content_disposition, ';');
	for (std::vector<std::string>::iterator it = string_tokens.begin(); it != string_tokens.end(); it++)
	{
		std::pair<std::string, std::string>	kv_pair;

		kv_pair = extract_key_value_pair(string_tokens[2], '=');
		if (kv_pair.first == key)
		{
			result = kv_pair.second;
			return result;
		}
	}
	return result;
}

}
