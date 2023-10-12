#pragma once 

#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "../utils/utils.hpp"
#include "../Exception/HTTPException.hpp"

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
		std::string							get_content() const;

		void								set_headers(const std::map<std::string, std::string> &headers);
		void								set_content(const std::string &content);
	
	private:
		std::map<std::string, std::string>	_headers;
		std::string							_content;

		std::string	_extract_value_from_content_disposition(const std::string &key) const;
};

}
