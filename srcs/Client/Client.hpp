#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctime>
#include <string>
#include <memory>
#include <sstream>
#include <map>
#include <cstring>

#include "../../includes/macros.hpp"
#include "../Configurations/webserv-configurations.hpp"
#include "../Exception/HTTPException.hpp"
#include "../HTTP/Request.hpp"
#include "../HTTP/Response.hpp"
#include "../CGI/CGI.hpp"
#include "../Configurations/CommonServerConfig.hpp"

namespace ft
{

class Client
{
	public:
		enum ClientState
		{
			READING_REQUEST_HEADERS,
			REQUEST_HEADERS_CONSTRUCTED,
			READING_REQUEST_BODY,
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
		ClientState					get_client_state() const;
		CommonServerConfig const&	get_common_server_config() const;
		std::string					get_server_ip() const;
		std::string					get_client_ip() const;
		int							get_server_port() const;
		int							get_client_port() const;

		void						set_server_config(ServerConfig *server_config);
		void						set_client_state(ClientState const &state);

		std::size_t					handle_request();
		void 						handle_response();
		std::size_t					read_to_buffer();
		void						handle_request_header_construction();
		void 						process_header();
		void						handle_request_body();
		void						check_request_body();


	private:
		static const std::map<std::string, std::string> mime_type_map;

		static const std::map<std::string, std::string>	_fill_mime_type_map();

		ClientState							_state;
		int									_fd;
		std::stringstream					_buffer_stream;
		std::auto_ptr<CommonServerConfig>	_common_server_config;
		Request								_request;
		Response							_response;
		std::size_t 						_buffer_size;
		std::string							_endpoint;
		std::string							_dir_path;
		std::auto_ptr<CGI>					_cgi;
		int									_client_port;
		std::string							_client_ip;
		int									_server_port;
		std::string							_server_ip;

		void								_match_location();
		void								_is_method_allowed();
		void								_handle_method();
		void								_handle_get(const std::string &path);
		void								_handle_post(const std::string &path);
		void								_handle_delete(const std::string &file_path);
		std::string							_generate_dir_content_list_html(const std::string &dir);
		void								_handle_exception();
		void								_handle_autoindex(const std::string &dir_path, std::stringstream &stream);
		void								_handle_redirect();
		bool								_is_target_cgi();
		void								_configure_common_config();
		void								_initialize_cgi();
		std::string							_extract_binary_path_from_config(const std::string &file_extension);
		std::string							_translate_binary_path();
		std::string 						_get_cgi_path_info();
		std::size_t 						_get_body_size();
		void 								_check_body_size_within_limits();
		bool								_has_full_message_format_received();
		bool								_has_received_complete_body();
		std::string							_identify_boundary_string();
		void								_create_request_body();
		void								_check_content_length_header_within_limits();
		void 								_parse_chunked_request_body(std::stringstream &stream);
		std::string							_identify_content_type();
		void								_check_request_body_headers();
		void								_construct_directory_path();

};

}
