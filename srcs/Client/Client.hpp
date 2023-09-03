#pragma once

#include "../../includes/webserv.hpp"

namespace ft
{

class Client
{
	public:
		enum ProcessState
		{
			PROCESSING_REQUEST,
			VALIDATING_REQUEST,
			PROCESSING_RESPONSE,
			PROCESSING_EXCEPTION,
			PROCESSING_CGI,
			SENDING_RESPONSE,
			FINISHED_PROCESSING
		};

		Client(int fd, unsigned int buffer_size);
		~Client();

		int							get_fd() const;
		Request						get_request() const;
		Response					get_response() const;
		ProcessState				get_process_state() const;
		ServerConfig const*			get_server_config() const;

		void						set_server_config(ServerConfig *server_config);
		void						set_process_state(ProcessState const &state);

		int							handle_request();
		void 						handle_response();

	private:
		static const std::map<std::string, std::string> mime_type_map;

		static const std::map<std::string, std::string>	_fill_mime_type_map();

		ProcessState				_state;
		int							_fd;
		const CommonServerConfig	*_common_server_config;
		ServerConfig				*_server_config;
		Request						_request;
		Response					_response;
		unsigned int 				_buffer_size;
		std::istream				*_content;
		std::string					_endpoint;
		CGI							*_cgi;

		void		_match_location(std::string location_str);
		void		_handle_auto_index(const std::string &dir);
		void		_is_method_allowed();
		void		_handle_method(const std::string &endpoint);
		void		_handle_get(const std::string &endpoint);
		void		_handle_post(const std::string &endpoint);
		void		_handle_delete(std::string file_path);
		std::string	_generate_dir_content_list_html(std::string dir);
		void		_handle_exception();
		void		_handle_auto_index(const std::string &dir_path, std::stringstream &stream);
		void		_handle_redirect();
		void		_handle_cgi(const std::string &cgi_script_name);
		bool		_is_target_cgi();
		char		**_prepare_cgi_meta_variables();
		void		_configure_common_config();
		void		_initialize_cgi();
		std::string _get_cgi_binary();
		std::string	_translate_binary_path();

};

}
