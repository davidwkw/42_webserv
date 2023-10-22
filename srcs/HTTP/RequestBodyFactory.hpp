#pragma once

#include <string>

#include "../Exception/HTTPException.hpp"
#include "RequestMultipart.hpp"
#include "RequestMultipartFactory.hpp"
#include "RequestBody.hpp"

namespace ft
{
	
class RequestBodyFactory
{
	public:
		RequestBodyFactory();
		RequestBodyFactory(const std::string &raw_content);
		~RequestBodyFactory();

		std::string		get_raw_content() const;
		void 			set_raw_content(const std::string &raw_content);

		void			reset();
		RequestBody 	*build_multipart(const std::string &boundary);
		RequestBody 	*build_form_encoded();

	private:
		std::string						_raw_content;

		std::map<int, RequestMultipart>	_extract_multipart_request_body(const std::string &boundary);
		std::string						_extract_form_encoded();
};

}
