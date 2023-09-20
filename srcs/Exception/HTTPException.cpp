#include "HTTPException.hpp"

namespace ft
{

HTTPException::HTTPException(int status_code, std::string message) : _status_code(status_code), _message(message){}

HTTPException::~HTTPException() throw(){}

const char* HTTPException::what() const throw()
{
	std::stringstream ss;

	ss << "Status code: " << this->_status_code << "\n" << "Message: " << this->_message << "\n";

	return ss.str().c_str();
}

int HTTPException::get_status_code() const
{
	return this->_status_code;
}


}
