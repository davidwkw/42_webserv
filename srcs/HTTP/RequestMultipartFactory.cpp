#include "RequestMultipartFactory.hpp"

namespace ft
{

RequestMultipartFactory::RequestMultipartFactory() : _raw_string(){}

RequestMultipartFactory::RequestMultipartFactory(const std::string &raw_string) : _raw_string(raw_string){}

RequestMultipartFactory::~RequestMultipartFactory(){}

RequestMultipart	*RequestMultipartFactory::build_request_multipart()
{
	RequestMultipart *result = new RequestMultipart();
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
		kv_pair = extract_key_value_pair(line, '=');
		result.insert(kv_pair);
	}
	return result;
}

std::string RequestMultipartFactory::_extract_body()
{
	std::size_t crlf_pos;
	std::string result;

	crlf_pos = this->_raw_string.find(CRLF CRLF);
	if (crlf_pos == 0) // header missing not allowed, Content-Disposition required at minimum according to rfc7578
	{
		throw std::runtime_error("Missing headers");
	}
	result = this->_raw_string.substr(crlf_pos + 4);
	return result;
}

}
