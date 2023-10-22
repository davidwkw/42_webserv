#include "CGI.hpp"

namespace ft
{

CGI::CGI() : _state(PROCESSING), _binary(), _args(), _envp(), _read_pipefd(STDIN_FILENO), _write_pipefd(STDOUT_FILENO), _pid(-1), _time_since_last_activity(time(NULL)), _output_stream(), _headers(), _response_status(200){}

CGI::CGI(const std::string &binary) : _state(PROCESSING), _binary(binary), _args(), _envp(), _read_pipefd(STDIN_FILENO), _write_pipefd(STDOUT_FILENO), _pid(-1), _time_since_last_activity(time(NULL)), _output_stream(), _headers(), _response_status(200){}

CGI::~CGI()
{
	close(this->_read_pipefd);
	close(this->_write_pipefd);
}

CGI::ProcessState CGI::get_state() const
{
	return this->_state;
}

pid_t CGI::get_pid() const
{
	return this->_pid;
}

int	CGI::get_write_fd() const
{
	return this->_write_pipefd;
}

int	CGI::get_read_fd() const
{
	return this->_read_pipefd;
}

time_t CGI::get_time_since_last_activity() const
{
	return this->_time_since_last_activity;
}


std::string CGI::get_body_string() const
{
	std::size_t header_pos;
	std::string	body_string;

	header_pos = this->_output_stream.str().find("\n\n");
	if (header_pos != std::string::npos)
	{
		body_string = this->_output_stream.str().substr(header_pos + 2);
	}
	return body_string;
}

const std::stringstream &CGI::get_output_stream() const
{
	return this->_output_stream;
}

std::stringstream &CGI::get_output_stream()
{
	return this->_output_stream;
}

std::map<std::string, std::string> CGI::get_headers()
{
	return this->_headers;
}

void CGI::update_time_since_last_activity(const time_t &time)
{
	this->_time_since_last_activity = time;
}

void CGI::set_pid(pid_t pid)
{
	this->_pid = pid;
}

void CGI::set_binary(const std::string &binary)
{
	this->_binary = binary;
}

void CGI::close_read_fd()
{
	if (this->_read_pipefd <= 2)
		return;
	close(this->_read_pipefd);
	this->_read_pipefd = STDIN_FILENO;
}

void CGI::close_write_fd()
{
	if (this->_write_pipefd <= 2)
		return;
	close(this->_write_pipefd);
	this->_write_pipefd = STDOUT_FILENO;
}

void CGI::add_arg(const std::string &arg)
{
	this->_args.push_back(arg);
}

void CGI::add_envp(std::string key, std::string val)
{
	str_to_uppercase(key);
	str_replace_char(key, '-', '_');
	this->_envp.insert(key + "=" + val);
}

void CGI::add_server_envp(const char *env_var)
{
	this->_envp.insert(std::string(env_var));
}

void CGI::execute()
{
	if (access(this->_binary.c_str(), X_OK) == -1)
	{
		throw HTTPException(500, "CGI not executable");
	}

	int		cgi_writepipe[2];
	int		cgi_readpipe[2];

	if (pipe(cgi_writepipe) == -1 || pipe(cgi_readpipe) == -1)
	{
		throw HTTPException(500, "Pipe error");
	}
	this->_pid = fork();
	if (this->_pid == -1)
	{
		throw HTTPException(500, "Fork failure");
	}
	else if (this->_pid == 0) // child
	{ 
		dup2(cgi_readpipe[0], STDIN_FILENO);
		close(cgi_readpipe[0]);
		dup2(cgi_writepipe[1], STDOUT_FILENO);
		close(cgi_writepipe[1]);

		std::auto_ptr<char *> meta_envp;
		meta_envp = this->_prepare_meta_variables();
		std::auto_ptr<char *> args;
		args = this->_prepare_cgi_arg();
		execve(this->_binary.c_str(), args.get(), meta_envp.get());
		std::exit(1);
	}
	else
	{
		close(cgi_readpipe[0]);
		close(cgi_writepipe[1]);
		this->_read_pipefd = cgi_writepipe[0];
		this->_write_pipefd = cgi_readpipe[1];
		this->_time_since_last_activity = time(NULL);
	}
}

void CGI::read_cgi_stream(const size_t &read_amount)
{
	std::auto_ptr<char> buffer(new char[read_amount]);
	ssize_t				bytes_read = 0;

	std::memset(buffer.get(), 0, read_amount);
	bytes_read = read(this->_read_pipefd, buffer.get(), read_amount);
	if (bytes_read == -1)
	{
		throw HTTPException(500, "Read failed");
	}
	else if (bytes_read == 0)
	{
		close(this->_read_pipefd);
		this->_read_pipefd = STDIN_FILENO;
		this->_state = FINISHED;
	}
	else
	{
		this->_output_stream << buffer.get();
	}
}

void	CGI::write_to_cgi(const std::string &str)
{
	write(this->_write_pipefd, str.c_str(), str.length());
}

void	CGI::update_state()
{
	int status;

	status = waitpid(this->_pid, NULL, WNOHANG);
	if (WIFEXITED(status) != 0)
	{
		status = WEXITSTATUS(status);
		if (status != 0)
		{
			throw std::runtime_error("Non-zero exit from forked process");	
		}
		this->_state = PROCESSING_COMPLETED;
	}
}

void CGI::process_output()
{
	std::size_t header_pos;

	header_pos = this->_output_stream.str().find("\n\n");
	if (header_pos == std::string::npos)
	{
		this->_response_status = 500;
		throw std::runtime_error("Couldn't find header end");	
	}
	this->_parse_headers();
	if (this->_headers.find("Location") != this->_headers.end()
		&& this->_headers.find("Content-Type") != this->_headers.end()) // redirect with document
	{
		if (this->_headers.find("Status") == this->_headers.end())
		{
			this->_response_status = 500;
			std::runtime_error("Missing status header");
		}
		std::stringstream	status_string;

		status_string.str(this->_headers.at("Status"));
		status_string >> this->_response_status;
		if (this->_response_status != 302)
		{
			this->_response_status = 500;
			std::runtime_error("Invalid status code");
		}
	}
	else if (this->_headers.find("Location") != this->_headers.end())
	{
		this->_response_status = 302;
	}
	else if (this->_headers.find("Content-Type") != this->_headers.end())
	{
		try
		{
			std::stringstream	status_string;

			status_string.str(this->_headers.at("Status"));
			status_string >> this->_response_status;
		}
		catch (const std::out_of_range &e){}
	}
	else 
	{
		this->_response_status = 500;
		throw std::runtime_error("Missing at least one header field");
	}
	this->_headers.erase("Status");
}

int	CGI::get_response_status() const
{
	return this->_response_status;
}

void CGI::_validate_header_field(const std::pair<std::string, std::string> &header_pair)
{
	if (header_pair.first == "")
	{
		throw std::runtime_error("Missing header key");
	}
	if (header_pair.first == "Content-Length")
	{
		char *end;
		long int content_length = std::strtol(header_pair.second.c_str(), &end, 10);

		if (content_length < 0)
		{
			throw std::runtime_error("Invalid Content-Length value");
		}
		else if (content_length == 0 && std::strcmp(end, "\0") != 0)
		{
			throw std::runtime_error("Invalid conversion");
		}
	}
}

void CGI::_parse_headers()
{
	std::string														line;
	std::size_t														separator_index;
	std::size_t														whitespace_index;
	std::pair<std::string, std::string>								header_field_pair;
	std::pair<std::map<std::string, std::string>::iterator,bool>	insert_return;

	while (std::getline(this->_output_stream, line) && !line.empty())
	{
		separator_index = line.find(':');
		whitespace_index = line.find_first_of(WHITESPACE_CHARACTERS);
		if (whitespace_index < separator_index)
		{
			throw std::runtime_error("Whitespace found before : during header parsing");
		}
		header_field_pair = extract_key_value_pair(line, ':');
		this->_validate_header_field(header_field_pair);
		insert_return = this->_headers.insert(header_field_pair);
    }
	getline_CRLF(this->_output_stream, line);
}

std::auto_ptr<char *> CGI::_prepare_meta_variables()
{
	std::auto_ptr<char *>	env_arr(new char*[this->_envp.size() + 1]);
	std::size_t				index = 0;

	for (std::set<std::string>::iterator it = this->_envp.begin(); it != this->_envp.end(); it++)
	{
		env_arr.get()[index] = const_cast<char *>(it->c_str());
		index++;
	}
	env_arr.get()[index] = NULL;
	return env_arr;
}

std::auto_ptr<char *> CGI::_prepare_cgi_arg()
{
	std::auto_ptr<char *> args(new char*[1 + this->_args.size() + 1]);
	std::size_t	index = 0;

	args.get()[index] = const_cast<char *>(this->_binary.c_str());
	index++;
	for (std::vector<std::string>::iterator it = this->_args.begin(); it != this->_args.end(); it++)
	{
		args.get()[index] = const_cast<char *>(it->c_str());
		index++;
	}
	args.get()[index] = NULL; 
	return args;
}

}
