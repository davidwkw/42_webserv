#pragma once

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <dirent.h>
#include <map>
#include <set>
#include <vector>
#include <sstream>
#include <string>
#include <time.h>
#include <memory>
#include <cstring>
#include <time.h>
#include <cstdlib>

#include "../../includes/macros.hpp"
#include "../utils/utils.hpp"
#include "../Exception/HTTPException.hpp"

namespace ft
{

class CGI
{
	public:
		enum ProcessState
		{
			PROCESSING,
			PROCESSING_COMPLETED,
			FINISHED
		};

		CGI();
		~CGI();

		void					add_arg(const std::string &arg);
		void 					add_envp(std::string key, std::string val);
		void					add_server_envp(const char *env_var);
		void 					execute();

		void					update_time_since_last_activity(const time_t &time);
		void					write_to_cgi(const std::string &str);
		void					read_cgi_stream(const size_t &val);
		void					update_state();
		void					close_write_fd();
		void					close_read_fd();
		void					process_output();

		std::map<std::string, std::string>	get_headers();
		time_t								get_time_since_last_activity() const;
		std::string							get_body_string() const;
		const std::stringstream 			&get_output_stream() const;
		std::stringstream					&get_output_stream();
		int									get_write_fd() const;
		int									get_read_fd() const;
		ProcessState						get_state() const;
		pid_t								get_pid() const;

		void								set_pid(pid_t pid);
		void								set_binary(const std::string &binary);

	private:
		ProcessState						_state;
		std::string							_binary;
		std::vector<std::string>			_args;
		std::set<std::string>				_envp;
		int									_read_pipefd;
		int									_write_pipefd;
		pid_t								_pid;
		time_t								_time_since_last_activity;
		std::stringstream					_output_stream;
		std::map<std::string, std::string>	_headers;

		void					_validate_header_field(const std::pair<std::string, std::string> &header_pair);
		void					_parse_headers();
		std::auto_ptr<char *>	_prepare_meta_variables();
		std::auto_ptr<char *>	_prepare_cgi_arg();

};

}
