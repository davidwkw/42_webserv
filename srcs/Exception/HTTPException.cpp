#include "HTTPException.hpp"

namespace ft
{

HTTPException::HTTPException(int status_code, std::string message) : _status_code(status_code), _message(message), _formatted_output()
{
	std::stringstream ss;

	ss << "Status code: " << this->_status_code << "\n" << "Message: " << this->_message << "\n";
	this->_formatted_output = ss.str();
}

HTTPException::~HTTPException() throw(){}

const char* HTTPException::what() const throw()
{
	return this->_formatted_output.c_str();
}

int HTTPException::get_status_code() const
{
	return this->_status_code;
}

}
