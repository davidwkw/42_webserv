#pragma once

#include <exception>
#include <string>
#include <sstream>

namespace ft
{

class HTTPException : public std::exception
{
	public:
		HTTPException(int status_code, std::string message);
		~HTTPException() throw();
		const char* what() const throw();

		int get_status_code() const;

	private:
		int _status_code;
		std::string _message;
};

}
