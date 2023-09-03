#ifndef __RESPONSE__
#define __RESPONSE__

#include "../../includes/webserv.hpp"

namespace ft 
{

class Response
{
	public:
		enum ResponseWriteState
		{
			WRITING,
			FINISHED
		};

	private:
		static const std::map<int, std::string>			reason_phrase_map;
		
		float								_http_protocol;
		int									_status_code;
		std::map<std::string, std::string>	_headers;
		std::stringstream					*_message_format;
		std::istream						*_body_stream;
		ResponseWriteState					_write_state;

	public:
		static const std::map<int, std::string> _fill_reason_phrase_map();

		Response();
		Response(float protocol_version);
		Response(const Response &src);
		Response &operator=(const Response &src);
		~Response();

		void				construct_response_message_format();
		std::string			read_response(std::size_t buffer_size);

		std::istream 		*get_message_format() const;
		std::istream const	*get_body_stream() const;
		std::istream 		*get_body_stream();
		ResponseWriteState	get_write_state() const;
		int					get_status_code() const;
		std::string 		get_string() const;

		void 				set_status_code(const int &code);
		void 				set_body_stream(std::istream &body);
		void				set_header(const std::map<std::string, std::string> &header_map);
		void				set_header(const std::string &key, const std::string &value);
		void				remove_header(const std::string &key);
};

}

#endif
