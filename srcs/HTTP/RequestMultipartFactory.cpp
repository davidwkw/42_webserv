#include "RequestMultipartFactory.hpp"

// best to throw custom exception defined within RequestMultipart. runtime_error doesn't provide enough info barely different from returning NULL;

namespace ft
{

RequestMultipartFactory::RequestMultipartFactory() : _raw_string(){}

RequestMultipartFactory::RequestMultipartFactory(const std::string &raw_string) : _raw_string(raw_string){}

RequestMultipartFactory::~RequestMultipartFactory(){}

RequestMultipart *RequestMultipartFactory::build_request_multipart() // might not be best to return null;
{
	RequestMultipart *result = NULL;

	if (this->_raw_string.length() == 0)
	{
		throw std::runtime_error("Unset raw string");
		return result;
	}

	result = new RequestMultipart();
	try 
	{
		result->set_headers(this->_extract_headers());
		result->set_content(this->_extract_body());
	}
	catch (const std::exception &e)
	{
		delete result;
		throw e;
	}
	return result;
}

void RequestMultipartFactory::reset()
{
	this->_raw_string = "";
}

std::string RequestMultipartFactory::get_raw_string() const
{
	return this->_raw_string;
}

void RequestMultipartFactory::set_raw_string(const std::string &raw_string)
{
	this->_raw_string = raw_string;
}

std::map<std::string, std::string> RequestMultipartFactory::_extract_headers()
{
	std::size_t 						crlf_pos;
	std::string							line;
	std::stringstream					header_stream(this->_raw_string);
	std::map<std::string, std::string>	result;

	crlf_pos = this->_raw_string.find(CRLF CRLF);
	if (crlf_pos == 0) // header missing not allowed, Content-Disposition required at minimum according to rfc7578
	{
		throw std::runtime_error("Missing headers");
	}
	header_stream.str(this->_raw_string.substr(this->_raw_string.find_first_not_of(CRLF), crlf_pos + 2));

	while (getline_CRLF(header_stream, line))
	{
		std::pair<std::string, std::string> kv_pair;

		line = trim_chars(line, WHITESPACE_CHARACTERS);
		kv_pair = extract_key_value_pair(line, ':');
		result.insert(kv_pair);
	}
	this->_validate_headers(result);
	return result;
}

std::string RequestMultipartFactory::_extract_body()
{
	std::size_t crlf_pos;
	std::string result;

	crlf_pos = this->_raw_string.find(CRLF CRLF);
	if (crlf_pos == 0) // missing header not allowed, Content-Disposition required at minimum according to rfc7578
	{
		throw std::runtime_error("Missing headers");
	}
	result = this->_raw_string.substr(crlf_pos + 4);
	return result;
}

void RequestMultipartFactory::_validate_headers(const std::map<std::string, std::string> &headers)
{
	if (headers.find("Content-Disposition") == headers.end())
	{
		throw std::runtime_error("Missing Content-Disposition header");
	}

	for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); it++)
	{
		if (it->first != "Content-Type" || it->first != "Content-Disposition")
		{
			continue;
		}
		if (it->first == "Content-Disposition")
		{
			this->_validate_content_disposition(it->second);
		}
	}
}

void RequestMultipartFactory::_validate_content_disposition(const std::string &content_disposition_line)
{
	std::string							str;
	std::stringstream					ss;
	std::size_t							delimiter_pos;
	std::pair<std::string, std::string> kv_pair;

	delimiter_pos = content_disposition_line.find_first_of(';');
	str = trim_chars(content_disposition_line.substr(0, delimiter_pos), WHITESPACE_CHARACTERS);
	if (str != "form-data")
	{
		throw std::runtime_error("Missing Content-Disposition header");
	}
	ss.str(content_disposition_line.substr(delimiter_pos));
	while (std::getline(ss, str, ';')) // about 3x slower than manual find solution
	{
		str = trim_chars(str, WHITESPACE_CHARACTERS);
		kv_pair = extract_key_value_pair(str, '=');
		if (kv_pair.first == "name")
		{
			return;
		}
	}
	throw std::runtime_error("Name field missing from content-disposition");
}

}
