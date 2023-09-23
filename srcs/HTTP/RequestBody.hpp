#pragma once

#include <string>
#include <map>
#include <sstream>
#include "../utils/utils.hpp"
#include "../../includes/macros.hpp"
#include "../Exception/HTTPException.hpp"
#include "RequestMultipart.hpp"

namespace ft
{

class RequestBody
{
	public:
		RequestBody();
		RequestBody(const std::string &type);
		~RequestBody();

		void	set_type(const std::string &type);
		void	set_index_multipart_map(const std::map<int, RequestMultipart> &multipart_map);
		void	set_encoded_form(const std::string &encoded_string);
		void	set_raw_stream(const std::string& str);
		
		std::string						get_type() const;
		std::string						get_encoded_form() const;
		std::map<int, RequestMultipart>	get_index_multipart() const;
		std::stringstream				&get_raw_stream();
		std::string						get_raw_string() const;

	private:
		std::string							_type;
		std::stringstream					_raw_stream;
		std::map<int, RequestMultipart>		_index_to_multipart;
		std::string							_encoded_form;
};

}
