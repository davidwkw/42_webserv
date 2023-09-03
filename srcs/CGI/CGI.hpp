#pragma once

#include "../../includes/webserv.hpp"

namespace ft
{

class CGI
{
	public:

		enum ProcessState
		{
			PROCESSING,
			READING,
			FINISHED
		};

		CGI(const std::string &binary);
		~CGI();

		void 					add_envp(std::string key, std::string val);
		void 					execute();

		void					update_time_since_last_activity(const time_t &time);
		void					write_to_cgi(const std::string &str);
		std::string				read_cgi_stream(const size_t &val);
		void					check_completed();
		void					close_write_fd();
		void					close_read_fd();
		void					process_output();

		std::map<std::string, std::string> get_headers();
		time_t					get_time_since_last_activity() const;
		const std::stringstream &get_body() const;
		std::stringstream		&get_body();
		const std::stringstream &get_output_stream() const;
		std::stringstream		&get_output_stream();
		int						get_write_fd() const;
		int						get_read_fd() const;
		ProcessState			get_state() const;
		pid_t					get_pid() const;

		void					set_pid(pid_t pid);

	private:
		std::map<std::string, std::string>	_headers;
		std::string							_binary;
		int									_read_pipefd;
		int									_write_pipefd;
		std::set<std::string>				_envp;
		std::stringstream					_output_stream;
		pid_t								_pid;
		time_t								_time_since_last_activity;
		ProcessState						_state;
		std::stringstream					_body;

		void					_validate_header_field(const std::pair<std::string, std::string> &header_pair);
		void					_parse_headers();
		void					_parse_body();

};

}
