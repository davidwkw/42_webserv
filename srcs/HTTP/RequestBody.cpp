#include "RequestBody.hpp"

namespace ft
{

RequestBody::RequestBody() : _type(), _index_to_multipart(), _encoded_form(){}

RequestBody::RequestBody(const std::string &type) : _type(type), _index_to_multipart(), _encoded_form(){}

RequestBody::~RequestBody(){}

void RequestBody::set_type(const std::string &type)
{
	this->_type = type;
}

void RequestBody::set_index_multipart_map(const std::map<int, RequestMultipart> &multipart_map)
{
	this->_index_to_multipart = multipart_map;
}

void RequestBody::set_encoded_form(const std::string &encoded_string)
{
	this->_encoded_form = encoded_string;
}

void RequestBody::set_raw_stream(const std::string& raw_str)
{
	this->_raw_stream.str(raw_str);
}

std::string	RequestBody::get_type() const
{
	return this->_type;
}

std::string	RequestBody::get_encoded_form() const
{
	if (this->_type != "application/x-www-form-urlencoded")
	{
		return "";
	}
	return this->_encoded_form;
}

std::map<int, RequestMultipart>	RequestBody::get_index_multipart() const
{
	if (this->_type != "multipart/form-data")
	{
		return std::map<int, RequestMultipart>();
	}
	return this->_index_to_multipart;
}

std::stringstream &RequestBody::get_raw_stream()
{
	return this->_raw_stream;
}

std::string RequestBody::get_raw_string() const
{
	return this->_raw_stream.str();
}

}
