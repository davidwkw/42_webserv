#pragma once

#include <string>
#include <sstream>

#include "macros.hpp"
#include "../utils/utils.hpp"
#include "RequestMultipart.hpp"

namespace ft
{

class RequestMultipartFactory
{
	public:
		RequestMultipartFactory();
		RequestMultipartFactory(const std::string &raw_string);
		~RequestMultipartFactory();

		std::string			get_raw_string() const;
		void				set_raw_string(const std::string &raw_string);

		RequestMultipart	*build_request_multipart();
		void				reset();

	private:
		std::string _raw_string;

		std::map<std::string, std::string>	_extract_headers();
		std::string							_extract_body();
		void								_validate_headers(const std::map<std::string, std::string> &headers);
		void								_validate_content_disposition(const std::string &content_disposition_line);

};

}
