#include "HTTPException.hpp"

namespace ft
{

HTTPException::HTTPException(unsigned int status_code, std::string message) : _status_code(status_code), _message(message)
{}

const char* HTTPException::what() const throw()
{
	std::stringstream ss;

	ss << "Status code: " << this->_status_code << "\n" << "Message: " << this->_message << "\n";

	return ss.str().c_str();
}

}
