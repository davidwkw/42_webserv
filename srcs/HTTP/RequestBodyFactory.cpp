#include "RequestBodyFactory.hpp"

namespace ft
{

RequestBodyFactory::RequestBodyFactory(){}

RequestBodyFactory::RequestBodyFactory(const std::string &raw_content) : _raw_content(raw_content){}

RequestBodyFactory::~RequestBodyFactory(){}

RequestBody *RequestBodyFactory::build_multipart(const std::string &boundary)
{
	RequestBody	*request_body;

	request_body = new RequestBody("multipart/form-data");
	request_body->set_raw_stream(this->_raw_content);
	request_body->set_index_multipart_map(this->_extract_multipart_request_body(boundary));
	return request_body;
}

RequestBody *RequestBodyFactory::build_form_encoded()
{
	RequestBody	*request_body;

	request_body = new RequestBody("application/x-www-form-urlencoded");
	request_body->set_raw_stream(this->_raw_content);
	request_body->set_encoded_form(this->_extract_form_encoded());
	return request_body;
}

std::string	RequestBodyFactory::get_raw_content() const
{
	return this->_raw_content;
}

void RequestBodyFactory::set_raw_content(const std::string &raw_content)
{
	this->_raw_content = raw_content;
}

std::map<int, RequestMultipart> RequestBodyFactory::_extract_multipart_request_body(const std::string &boundary)
{
	std::string	extracted_string = "";
	std::size_t pos	= 0;
	std::string	temp = this->_raw_content;
	std::map<int, RequestMultipart> result;

	if (temp.rfind(boundary + "--") == std::string::npos)
	{
		throw HTTPException(400, "Missing end body boundary" );
	}
	
	try
	{
		extracted_string = extract_between_boundaries(temp, boundary, boundary);
		pos = boundary.length() + extracted_string.length();
		while (extracted_string.length() > 0)
		{
			result.insert(std::make_pair(result.size(), trim_chars(extracted_string, WHITESPACE_CHARACTERS)));
			temp = temp.substr(pos);
			extracted_string = extract_between_boundaries(temp, boundary, boundary);
			pos = boundary.length() + extracted_string.length();
		}
	}
	catch (std::exception &e)
	{
		throw e; // placeholder in case constructing requestmultipart fails
	}
	return result;
}

std::string RequestBodyFactory::_extract_form_encoded()
{
	std::string result;

	result = this->_raw_content;
	return result;
}

}
