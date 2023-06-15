#ifndef __RESPONSE__
#define __RESPONSE__

#include "../../includes/webserv.hpp"
#include <string>

namespace ft 
{

class Response
{
	private:
		int _status_code;
		std::string body;

	public:
		Response();
		Response(const Response &src);
		Response &operator=(const Response &src);
		Response(const std::string &res_string);
		~Response();
		operator std::string() const;

		Response &status_code(int code);
		std::string build_response();

};

}

#endif
