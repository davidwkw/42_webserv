#include "RequestMultipart.hpp"

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
	return trim_chars(this->_extract_value_from_content_disposition("filename"), "\"");
}

std::string RequestMultipart::get_name() const
{
	std::string name;

	name = this->_extract_value_from_content_disposition("name");
	return name;
}

std::string RequestMultipart::get_content_type() const
{
	std::string result;

	try
	{
		result = this->_headers.at("Content-Type");
	}
	catch (const std::out_of_range &e)
	{
		result = "";
	}
	return result;
}

std::map<std::string, std::string> RequestMultipart::get_headers() const
{
	return this->_headers;
}

std::string RequestMultipart::get_content() const
{
	return this->_content;
}

void RequestMultipart::set_headers(const std::map<std::string, std::string> &headers)
{
	this->_headers = headers;
}

void RequestMultipart::set_content(const std::string &content)
{
	this->_content = content;
}

std::string	RequestMultipart::_extract_value_from_content_disposition(const std::string &key) const
{
	std::string							header_value;
	std::vector<std::string>			string_tokens;
	std::string							result;

	try
	{	
		header_value = this->_headers.at("Content-Disposition");
		string_tokens = tokenise_str(header_value, ';');
		for (std::vector<std::string>::iterator it = string_tokens.begin(); it != string_tokens.end(); it++)
		{
			std::pair<std::string, std::string>	kv_pair;

			kv_pair = extract_key_value_pair(string_tokens[2], '=');
			if (kv_pair.first == "")
			{
				continue;
			}
			if (kv_pair.first == key)
			{
				result = kv_pair.second;
				return result;
			}
		}
		return result;
	}
	catch(const std::out_of_range& e)
	{
		return "";
	}
}

}
