#pragma once

#include <exception>
#include <string>
#include <sstream>

namespace ft
{

class HTTPException : public std::exception
{
	private:
		unsigned int _status_code;
		std::string _message;

	public:
		HTTPException(unsigned int status_code, std::string message);
		const char* what() const throw();

		unsigned int get_status_code() const;
};

};
