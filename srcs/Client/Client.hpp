#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <sstream>
#include <map>
#include "../../includes/macros.hpp"
#include "../Configurations/webserv-configurations.hpp"
#include "../Exception/HTTPException.hpp"
#include "../HTTP/Request.hpp"
#include "../HTTP/Response.hpp"
#include "../CGI/CGI.hpp"

namespace ft
{

class CGI;
class CommonServerConfig;

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

		Client(int fd, std::size_t buffer_size);
		~Client();

		int							get_fd() const;
		Request const				&get_request() const;
		Response const				&get_response() const;
		Request						&get_request();
		Response					&get_response();
		ProcessState				get_process_state() const;
		ServerConfig const*			get_server_config() const;
		std::string					get_server_ip() const;
		std::string					get_client_ip() const;
		int							get_server_port() const;
		int							get_client_port() const;

		void						set_server_config(ServerConfig *server_config);
		void						set_process_state(ProcessState const &state);

		int							handle_request();
		void 						handle_response();

	private:
		static const std::map<std::string, std::string> mime_type_map;

		static const std::map<std::string, std::string>	_fill_mime_type_map();

		ProcessState				_state;
		int							_fd;
		CommonServerConfig			*_common_server_config;
		ServerConfig				*_server_config;
		Request						_request;
		Response					_response;
		std::size_t 				_buffer_size;
		std::string					_endpoint;
		std::string					_dir_path;
		std::auto_ptr<CGI>			_cgi;
		int							_client_port;
		std::string					_client_ip;
		int							_server_port;
		std::string					_server_ip;

		void						_match_location();
		void						_handle_auto_index(const std::string &dir);
		void						_is_method_allowed();
		void						_handle_method();
		void						_handle_get(const std::string &path);
		void						_handle_post(const std::string &path);
		void						_handle_delete(std::string file_path);
		std::string					_generate_dir_content_list_html(std::string dir);
		void						_handle_exception();
		void						_handle_auto_index(const std::string &dir_path, std::stringstream &stream);
		void						_handle_redirect();
		void						_handle_cgi(const std::string &cgi_script_name);
		bool						_is_target_cgi();
		void						_configure_common_config();
		void						_initialize_cgi();
		std::string					_extract_binary_path_from_config(const std::string &file_extension);
		std::string					_translate_binary_path();
		std::string 				_get_cgi_path_info();

};

}
