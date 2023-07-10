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
			PROCESSING_RESPONSE,
			PROCESSING_EXCEPTION,
			FINISHED_PROCESSING
		};

		Client(int fd, unsigned int buffer_size);
		~Client();

		int							get_fd() const;
		Request						get_request() const;
		Response					get_response() const;

		void						set_server_config(const ServerConfig &server_config);
		void						set_status_code(int code);
		void						set_process_state(const ProcessState &state);

		int							handle_request();
		void 						handle_response();
		ProcessState				get_process_state() const;

	private:
		ProcessState		_state;
		int					_fd;
		ServerConfig		_server_config;
		Request				_request;
		Response			_response;
		int					_status_code;
		unsigned int 		_buffer_size;
		std::ifstream		_response_file_stream;
		std::ofstream		_request_file_stream;

		std::string	_match_location(std::string location_str);
		void		_handle_auto_index(const std::string &dir);
		void		_is_method_allowed(const std::string &endpoint);
		void		_handle_method(std::string endpoint);
		void		_handle_get(const std::string &endpoint);
		void		_handle_post(const std::string &endpoint);
		void		_handle_delete(const std::string &endpoint);
		// std::string	_generate_dir_content_list_html(const std::string &dir);
		void		_handle_exception();
};

}
