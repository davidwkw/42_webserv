#pragma once 

#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "../utils/utils.hpp"

namespace ft
{
	
class RequestMultipart
{
	public:
		RequestMultipart();
		RequestMultipart(const std::map<std::string, std::string> &headers, const std::string &content);
		RequestMultipart(const RequestMultipart& ref);
		RequestMultipart &operator=(const RequestMultipart& ref);
		~RequestMultipart();

		std::string							get_filename() const;
		std::string 						get_name() const;
		std::string 						get_content_type() const;
		std::map<std::string, std::string>	get_headers() const;

		void								set_headers(const std::map<std::string, std::string> &headers);
		void								set_content(const std::string &content);
	
	private:
		std::map<std::string, std::string>	_headers;
		std::string							_content;

		std::string	_extract_key_from_header(const std::string &header_name, const std::string &key) const;
};

}
