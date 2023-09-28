#pragma once

#include "../utils/utils.hpp"
#include "../../includes/macros.hpp"

#include <string>
#include <map>
#include <vector>
#include <utility>
#include <algorithm>
#include <sstream>

namespace ft
{

	class RequestLine
	{
		public:
			RequestLine();
			RequestLine(const std::string &request_line);
			~RequestLine();
			RequestLine(const RequestLine& ref);
			RequestLine &operator=(const RequestLine& ref);

			void								construct();
			void								reset();

			std::string							get_start_line() const;
			std::string 						get_method() const;
			std::string 						get_target() const;
			std::string 						get_protocol() const;
			float								get_protocol_version() const;
			std::string 						get_target_file() const;
			std::map<std::string, std::string>	get_query_map() const;
			std::string							get_decoded_target() const;

			void								set_start_line(const std::string &line);
			void								set_method(const std::string &method);
			void								set_target(const std::string &target);
			void								set_protocol(const std::string &protocol);

		private:
			std::string							_start_line;
			std::string							_method; 
			std::string							_target;
			std::string							_protocol;

	};
	
}
