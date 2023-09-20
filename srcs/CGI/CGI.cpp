#include "CGI.hpp"

namespace ft
{

CGI::CGI() : _state(PROCESSING), _binary(), _args(), _envp(), _read_pipefd(STDIN_FILENO), _write_pipefd(STDOUT_FILENO), _pid(-1), _time_since_last_activity(time(NULL)), _output_stream(), _headers(){}

CGI::~CGI()
{
	close(this->_read_pipefd);
	close(this->_write_pipefd);
}

void CGI::_validate_header_field(const std::pair<std::string, std::string> &header_pair)
{
	if (header_pair.first == "Content-Length")
	{
		char *end;
		long int content_length = std::strtol(header_pair.second.c_str(), &end, 10);

		if (content_length < 0)
			throw std::runtime_error("Invalid Content-Length value");
		else if (content_length == 0 && std::strcmp(end, "\0") != 0)
			throw std::runtime_error("Invalid conversion");
	}
}

void CGI::_parse_headers()
{
	std::string														line;
	std::size_t														separator_index;
	std::size_t														whitespace_index;
	std::pair<std::string, std::string>								header_field_pair;
	std::pair<std::map<std::string, std::string>::iterator,bool>	insert_return;

	while (getline_CRLF(this->_output_stream, line) && !line.empty())
	{
		separator_index = line.find(':');
		whitespace_index = line.find_first_of(" \n\t\f\r\v");
		if (whitespace_index < separator_index)
			throw std::runtime_error("Whitespace found before : during header parsing");
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

	header_pos = this->_output_stream.str().find(CRLF CRLF);
	if (header_pos != std::string::npos)
	{
		body_string = this->_output_stream.str().substr(header_pos + 4);
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

	int		readpipe[2];
	int		writepipe[2];

	if (pipe(readpipe) == -1 || pipe(writepipe) == -1)
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
		dup2(writepipe[0], STDIN_FILENO);
		close(writepipe[0]);
		dup2(readpipe[1], STDOUT_FILENO);
		close(readpipe[1]);

		std::auto_ptr<char *> meta_envp;
		meta_envp = this->_prepare_meta_variables();
		std::auto_ptr<char *> args;
		args = this->_prepare_cgi_arg();

		execve(this->_binary.c_str(), args.get(), meta_envp.get());
		throw HTTPException(500, "Execve error");
	}
	else
	{
		close(readpipe[1]);
		close(writepipe[0]);
		this->_read_pipefd = readpipe[0];
		this->_write_pipefd = writepipe[1];
		this->_time_since_last_activity = time(NULL);
	}
}

void CGI::read_cgi_stream(const size_t &read_amount)
{
	std::auto_ptr<char> buffer(new char[read_amount + 1]);
	ssize_t				bytes_read = 0;

	std::memset(buffer.get(), 0, read_amount + 1);
	bytes_read = read(this->_read_pipefd, buffer.get(), sizeof(read_amount));
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

void	CGI::check_completed()
{
	int status;

	status = waitpid(this->_pid, NULL, WNOHANG);
	if (WIFEXITED(status))
	{
		status = WEXITSTATUS(status);
		if (status != 0)
		{
			throw std::runtime_error("Non-zero exit from forked process");	
		}
		this->_state = READING;
	}
}

void CGI::process_output()
{
	std::size_t header_pos;

	header_pos = this->_output_stream.str().find(CRLF CRLF);
	if (header_pos == std::string::npos)
	{
		throw std::runtime_error("Couldn't find header end");	
	}
	this->_parse_headers();
	this->_headers.insert(std::make_pair("Content-Length", to_string(this->get_body_string().length())));
}

}
