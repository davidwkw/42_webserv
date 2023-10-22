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
	request_body->set_index_multipart_map(this->_extract_multipart_request_body("--" + boundary));
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

void RequestBodyFactory::reset()
{
	this->_raw_content = "";
}

std::map<int, RequestMultipart> RequestBodyFactory::_extract_multipart_request_body(const std::string &boundary)
{
	std::string	extracted_string = "";
	std::size_t pos	= 0;
	std::string	temp = this->_raw_content;
	std::map<int, RequestMultipart> result;
	std::size_t	first_boundary_pos;
	std::size_t	end_boundary_pos;

	first_boundary_pos = temp.find(boundary); // necessary to ignore preamble
	if (first_boundary_pos != 0) // preamble present
	{
		temp = temp.substr(pos);
	}
	temp = std::string(CRLF) + temp;
	end_boundary_pos = temp.rfind(std::string(CRLF) + boundary + "--");
	if (end_boundary_pos == std::string::npos)
	{
		throw HTTPException(400 ,"Missing end boundary");
	}
	
	try
	{
		extracted_string = extract_between_boundaries(temp, std::string(CRLF) + boundary, std::string(CRLF) + boundary);
		pos = 2 + boundary.length() + extracted_string.length();
		while (extracted_string.length() > 0)
		{
			extracted_string = extracted_string.substr(extracted_string.find_first_not_of(WHITESPACE_CHARACTERS) - 2); // accounting for transport padding, assuming last 2 characters are CRLF
			result.insert(std::make_pair(result.size(), *RequestMultipartFactory(extracted_string).build_request_multipart())); // TODO: change to ptr storage
			if (pos >= end_boundary_pos) // ignore epilogue
			{
				break; 
			}
			temp = temp.substr(pos);
			extracted_string = extract_between_boundaries(temp, std::string(CRLF) + boundary, std::string(CRLF) + boundary);
			pos = 2 + boundary.length() + extracted_string.length();
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
