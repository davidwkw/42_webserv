#include "Client.hpp"

namespace ft
{
	
#pragma region Constructors

Client::Client(int fd, unsigned int buffer_size) : _fd(fd), _buffer_size(buffer_size), _state(PROCESSING_REQUEST), _request(), _server_config(), _status_code(), _response(HTTP_PROTOCOL, _status_code), _response_file_stream(), _request_file_stream()
{}

Client::~Client()
{
	if (this->_request_file_stream.is_open())
		this->_request_file_stream.close();
	if (this->_request_file_stream.is_open())
		this->_response_file_stream.close();
	// remember to delete autoindex file;
	// if (endpoint location was enable remove temp auto index file)
	// remove(path);
}

#pragma endregion Constructors

#pragma region ClassUtilities

void Client::_handle_method(std::string endpoint)
{
	std::string dir_path;
	std::string root;

	root = endpoint.empty() ? this->_server_config.root() : this->_server_config.locations().find(endpoint)->second.root();
	endpoint = endpoint.empty() ? "/" : endpoint;
	dir_path = root + endpoint;

	if (this->_request.get_method() == "GET")
		this->_handle_get(dir_path + this->_request.get_target());
	else if (this->_request.get_method() == "POST")
		this->_handle_post(endpoint);
	else if (this->_request.get_method() == "DELETE")
		this->_handle_delete(dir_path + this->_request.get_target());
}

std::string Client::_match_location(std::string endpoint_str)
{
	std::size_t slash_index = endpoint_str.find_last_of('/');
	while (slash_index != std::string::npos)
	{
		if (this->_server_config.locations().find(endpoint_str) != this->_server_config.locations().end())
			return endpoint_str;
		slash_index = endpoint_str.find_last_of('/', slash_index);
		endpoint_str = endpoint_str.substr(0, slash_index + 1);
	}
	return "";
}

void Client::_is_method_allowed(const std::string &endpoint)
{
	if (endpoint.empty())
		return;

	LocationConfig location_config = this->_server_config.locations().find(endpoint)->second;

	if (location_config.limit_except().find(this->_request.get_method()) == location_config.limit_except().end())
		throw HTTPException(405, "Method not allowed");
}

void Client::_handle_get(const std::string &file_path)
{
	if (!this->_request.get_target().empty())
		this->_request_file_stream.open(file_path.c_str());
	else if (this->_server_config.autoindex() == "on")
		this->_handle_auto_index(file_path);
	else
	{
		const std::vector<std::string> indexs = this->_server_config.locations().find(file_path)->second.index();

		for (std::vector<std::string>::const_iterator cit = indexs.begin(); cit != indexs.end(); cit++)
		{
			this->_request_file_stream.open((file_path + *cit).c_str());
			if (this->_request_file_stream.is_open())
				break;
		}
	}
	if (!this->_request_file_stream.is_open())
		throw HTTPException(404, "File not found");
	this->_status_code = 200;
}

// std::string Client::_generate_dir_content_list_html(const std::string &dir)
// {
// 	std::ostringstream	oss;
//     struct dirent		*entry;
// 	DIR					*folder;

//     folder = opendir(dir.c_str());
//     if(folder == NULL)
//         throw HTTPException(404, "Path non-existent");
// 	while ((entry = readdir(folder)) != NULL)
// 	{
// 		std::string path = this->_request.get_uri();

// 		if (entry->d_type == DT_DIR)
// 		{
// 			if (strcmp(entry->d_name, "..") == 0)
// 				path = path.substr(0, path.find_last_of('/', path.find_last_of('/')) + 1);
// 			oss << "<a href=\"/" << path << "\">" << entry->d_name << "</a>\n";
// 		}
// 		else if (entry->d_type == DT_REG)
// 			oss << "<a href=\"/" << (path + entry->d_name) << "\">" << entry->d_name << "</a>\n";
// 	}
// 	closedir(folder);
// 	return oss.str();
// }

void Client::_handle_auto_index(const std::string &dir)
{
	std::ostringstream	temp_autoindex_path;
	std::ifstream		auto_index_template;
	std::ofstream		output;
	std::string			line;

	auto_index_template.open(AUTOINDEX_TEMPLATE_PATH); // should probably be a directive in the config file..
	if (!auto_index_template.is_open())
		HTTPException(500, "Critical mess up. Couldn't open autoindex.html");
	temp_autoindex_path << "/public/temp_autoindex/fd_" << this->_fd << "_auto_index";
	output.open(temp_autoindex_path.str().c_str());
	if (!output.is_open())
		HTTPException(500, "Critical mess up. Couldn't create " + temp_autoindex_path.str());
	while (std::getline(auto_index_template, line))
	{
		std::size_t identifier_pos;

		if ((identifier_pos = line.find(DIRECTORY_TEMPLATE_IDENTIFIER)) != std::string::npos)
		{
			output << line.substr(0, identifier_pos);
			output << dir;
			output << line.substr(dir.length());
		}
		else if ((identifier_pos = line.find(DIRECTORY_CONTENT_TEMPLATE_IDENTIFIER)) != std::string::npos)
		{
			output << this->_generate_dir_content_list_html(dir);
		}
		output << line;
	}
	auto_index_template.close();
	output.close();
	this->_response_file_stream.open(temp_autoindex_path.str().c_str());
}

void Client::_handle_post(const std::string &endpoint)
{
	this->_status_code = 201;
}

void Client::_handle_delete(const std::string &endpoint)
{
	this->_status_code = 204;
}

void Client::_handle_exception()
{
	std::string								file_name = "";
	std::vector<std::vector<std::string> >	error_pages;
	std::ostringstream						oss;

	oss << this->_status_code;
	error_pages = this->_server_config.locations().find(endpoint)->second.error_page();
	for (std::vector<std::vector<std::string> >::const_iterator cit = error_pages.begin(); cit != error_pages.end(); cit++)
	{
		for (std::size_t index = 0; index < (cit->size() - 1); index++)
		{
			if (cit[index] == oss.str())
			{
				file_name = "/" + cit[index];
				break;
			}
		}
		if (!file_name.empty())
			break;
	}
	if (file_name.empty())
		return;
	this->_response_file_stream.open(("/public/" + file_name).c_str());
	if (!this->_response_file_stream.is_open())
		std::runtime_error("YOU'VE MESSED UP SOMEHOW");
}

#pragma endregion ClassUtilities

#pragma region Getters

int	Client::get_fd() const
{
	return this->_fd;
}

Request	Client::get_request() const
{
	return this->_request;
}

Response Client::get_response() const
{
	return this->_response;
}

Client::ProcessState Client::get_process_state() const
{
	return this->_state;
}

#pragma endregion Getters

#pragma region Setters

void Client::set_server_config(const ServerConfig &server_config)
{
	this->_server_config = server_config;
}

void Client::set_process_state(const ProcessState &state)
{
	this->_state = state;
}

#pragma endregion Setters

#pragma region PublicMemberMethods

int Client::handle_request()
{
	char *	buffer = static_cast<char*>(calloc(this->_buffer_size, sizeof(char)));

	if (recv(this->_fd, buffer, BUFFER_SIZE - 1, MSG_NOSIGNAL) <= 0)
	{
		free(buffer);
		return -1;
	}
	this->_request.append_to_request(buffer);
	free(buffer);
	try
	{
		this->_request.process_request();
	}
	catch(const HTTPException& e)
	{
		this->_status_code = e.get_status_code();
	}
	if (this->_request.get_request_read_state() == Request::FINISHED)
		this->_state = PROCESSING_RESPONSE;
}

void Client::handle_response()
{
	std::string		endpoint;
	LocationConfig	location_config;

	if (this->_status_code == 0)
	{
		endpoint = this->_match_location(this->_request.get_uri());
		try
		{
			if (endpoint.empty() && this->_server_config.root().empty()) // this shouldn't be possible; but just in case
				throw HTTPException(404, "Resource not found");
			this->_is_method_allowed(endpoint);
			this->_handle_method(endpoint);
		}
		catch (const HTTPException &e)
		{
			if (this->_response_file_stream.is_open())
				this->_response_file_stream.close();
			this->_status_code = e.get_status_code();
		}
	}
	else if ()
	else if ((this->_status_code < 200 || this->_status_code >= 300)
			&& this->_state == PROCESSING_RESPONSE)
	{
		this->_state = PROCESSING_EXCEPTION;
		this->_handle_exception();
	}
	// make the response
	// format and send the header,
	// read through open file into buffer and send;

	this->_response.set_status_code(this->_status_code);
	send(this->_fd, this->_response.get_string().c_str(), this->_response.get_string().length(), MSG_NOSIGNAL);
	if (this->_response.get_write_state() == Response::FINISHED)
	{
		this->_state = FINISHED_PROCESSING;
	}
}

#pragma endregion PublicMemberMethods

}
