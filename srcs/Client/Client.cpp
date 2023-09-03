#include "Client.hpp"

namespace ft
{

#pragma region Non-memberVariable

const std::map<std::string, std::string> Client::mime_type_map = Client::_fill_mime_type_map();

#pragma endregion Non-memberVariable

#pragma region Non-memberFunction

const std::map<std::string, std::string> Client::_fill_mime_type_map()
{
	std::map<std::string, std::string> container;

	container["aac"] = "audio/aac";
	container["abw"] = "application/x-abiword";
	container["arc"] = "application/octet-stream";
	container["avi"] = "video/x-msvideo";
	container["azw"] = "application/vnd.amazon.ebook";
	container["bin"] = "application/octet-stream";
	container["bz"] = "application/x-bzip";
	container["bz2"] = "application/x-bzip2";
	container["csh"] = "application/x-csh";
	container["css"] = "text/css";
	container["csv"] = "text/csv";
	container["doc"] = "application/msword";
	container["docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	container["eot"] = "application/vnd.ms-fontobject";
	container["epub"] = "application/epub+zip";
	container["gif"] = "image/gif";
	container["htm"] = "text/html";
	container["html"] = "text/html";
	container["ico"] = "image/x-icon";
	container["ics"] = "text/calendar";
	container["jar"] = "application/java-archive";
	container["jpeg"] = "image/jpeg";
	container["jpg"] = "image/jpeg";
	container["js"] = "application/javascript";
	container["json"] = "application/json";
	container["mid"] = "audio/midi";
	container["midi"] = "audio/midi";
	container["mp4"] = "video/mp4";
	container["mpeg"] = "video/mpeg";
	container["mpkg"] = "application/vnd.apple.installer+xml";
	container["odp"] = "application/vnd.oasis.opendocument.presentation";
	container["ods"] = "application/vnd.oasis.opendocument.spreadsheet";
	container["odt"] = "application/vnd.oasis.opendocument.text";
	container["oga"] = "audio/ogg";
	container["ogv"] = "video/ogg";
	container["ogx"] = "application/ogg";
	container["otf"] = "font/otf";
	container["png"] = "image/png";
	container["pdf"] = "application/pdf";
	container["ppt"] = "application/vnd.ms-powerpoint";
	container["pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	container["rar"] = "application/x-rar-compressed";
	container["rtf"] = "application/rtf";
	container["sh"] = "application/x-sh";
	container["svg"] = "image/svg+xml";
	container["swf"] = "application/x-shockwave-flash";
	container["tar"] = "application/x-tar";
	container["tif"] = "image/tiff";
	container["tiff"] = "image/tiff";
	container["ts"] = "application/typescript";
	container["ttf"] = "font/ttf";
	container["vsd"] = "application/vnd.visio";
	container["wav"] = "audio/x-wav";
	container["weba"] = "audio/webm";
	container["webm"] = "video/webm";
	container["webp"] = "image/webp";
	container["woff"] = "font/woff";
	container["woff2"] = "font/woff2";
	container["xhtml"] = "application/xhtml+xml";
	container["xls"] = "application/vnd.ms-excel";
	container["xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	container["xml"] = "application/xml";
	container["xul"] = "application/vnd.mozilla.xul+xml";
	container["zip"] = "application/zip";
	container["3gp"] = "video/3gpp";
	container["3g2"] = "video/3gpp2";
	container["7z"] = "application/x-7z-compressed";
	return container;
}

#pragma endregion Non-memberFunction

#pragma region Constructors

Client::Client(int fd, unsigned int buffer_size) : _fd(fd), _buffer_size(buffer_size), _state(PROCESSING_REQUEST), _request(), _server_config(NULL), _response(HTTP_PROTOCOL), _cgi(NULL), _content(NULL){}

Client::~Client()
{
	delete this->_cgi;
	delete this->_content;
}

#pragma endregion Constructors

#pragma region ClassUtilities

void Client::_handle_method(const std::string &endpoint)
{
	std::string dir_path;
	std::string root;

	root = this->_common_server_config->root();
	dir_path = root + (endpoint.empty() ? "/" : endpoint); 
	if (this->_request.get_method() == "GET")
		this->_handle_get(dir_path + this->_request.get_target());
	else if (this->_request.get_method() == "POST")
		this->_handle_post(dir_path);
	else if (this->_request.get_method() == "DELETE")
		this->_handle_delete(dir_path + this->_request.get_target());
}

void Client::_match_location(std::string endpoint_str)
{
	std::size_t slash_index = endpoint_str.find_last_of('/');

	while (slash_index != std::string::npos)
	{
		if (this->_server_config->locations().find(endpoint_str) != this->_server_config->locations().end())
		{
			this->_endpoint = endpoint_str;
		}
		if (slash_index == 0)
		{
			break;
		}
		slash_index = endpoint_str.find_last_of('/', slash_index - 1); // this is also broken
		endpoint_str = endpoint_str.substr(0, slash_index + 1);
	}
	this->_endpoint = "";
}

void Client::_is_method_allowed()
{
	if (this->_common_server_config->limit_except().find(this->_request.get_method()) == this->_common_server_config->limit_except().end())
	{
		throw HTTPException(405, "Method not allowed");
	}
}

std::string Client::_generate_dir_content_list_html(std::string dir_path)
{
	std::ostringstream	oss;
    struct dirent		*entry;
	DIR					*folder;

    folder = opendir(dir_path.c_str());
    if(folder == NULL)
        throw std::runtime_error("Couldn't open dir " + dir_path);
	while ((entry = readdir(folder)) != NULL)
	{
		std::string	link;
		std::string root_str = this->_common_server_config->root();
		
		link = this->_request.get_header("Host") + (dir_path.erase(dir_path.find(root_str), root_str.length())) + entry->d_name;
		if (entry->d_type == DT_REG)
			oss << "<a href=\"" << link.c_str() << "\">" << entry->d_name << "</a>\n"; 
	}
	closedir(folder);
	return oss.str();
}

void Client::_handle_auto_index(const std::string &dir, std::stringstream &stream)
{
	std::ostringstream	temp_autoindex_path;
	std::ifstream		auto_index_template;
	std::string			line;

	auto_index_template.open(AUTOINDEX_TEMPLATE_PATH); // should probably be a directive in the config file..
	if (!auto_index_template.is_open())
		HTTPException(500, "Critical mess up. Couldn't open autoindex.html");
	while (std::getline(auto_index_template, line))
	{
		std::size_t identifier_pos;

		if ((identifier_pos = line.find(DIRECTORY_TEMPLATE_IDENTIFIER)) != std::string::npos)
		{
			stream << line.substr(0, identifier_pos);
			stream << dir;
			stream << line.substr(dir.length());
		}
		else if ((identifier_pos = line.find(DIRECTORY_CONTENT_TEMPLATE_IDENTIFIER)) != std::string::npos)
		{
			stream << this->_generate_dir_content_list_html(dir);
		}
		stream << line << "\n";
	}
	auto_index_template.close();
}

void Client::_handle_get(const std::string &file_path)
{
	std::string											file_extension;
	std::map<std::string, std::string>::const_iterator	mime_type_iterator;

	if (!this->_request.get_target().empty())
	{
		std::ifstream *temp_stream = new std::ifstream;

		temp_stream->open(file_path.c_str());
		if (!temp_stream->is_open())
			throw HTTPException(404, "File not found");
		file_extension = extract_file_extension(this->_request.get_target());
		if ((mime_type_iterator = Client::mime_type_map.find(file_extension)) != Client::mime_type_map.end())
		{
			this->_response.set_header("Content-Type", mime_type_iterator->second);
		}
		this->_response.set_header("Content-Length", size_t_to_string(calc_input_stream_size(*temp_stream)));
		this->_response.set_body_stream(*temp_stream);
		this->_content = static_cast<std::istream *>(temp_stream);
	}
	else if (this->_server_config->autoindex() == "on")
	{
		std::stringstream *temp_stream = new std::stringstream;

		this->_handle_auto_index(file_path, *temp_stream);
		this->_response.set_header("Content-Type", "text/html"); // had to hardcode..
		this->_response.set_header("Content-Length", size_t_to_string(calc_input_stream_size(*temp_stream)));
		this->_response.set_body_stream(*temp_stream);
		this->_content = static_cast<std::istream *>(temp_stream);
	}
	else
	{
		const std::vector<std::string> indexs = this->_common_server_config->index();
		std::ifstream *temp_stream = new std::ifstream;

		for (std::vector<std::string>::const_iterator cit = indexs.begin(); cit != indexs.end(); cit++)
		{
			temp_stream->open((file_path + *cit).c_str());
			if (temp_stream->is_open())
			{
				file_extension = extract_file_extension(file_path + *cit);
				if ((mime_type_iterator = Client::mime_type_map.find(file_extension)) != Client::mime_type_map.end())
				{
					this->_response.set_header("Content-Type", mime_type_iterator->second);
				}
				break;
			}
		}
		if (!temp_stream->is_open())
		{
			delete temp_stream;
			throw HTTPException(404, "File not found");
		}
		this->_response.set_header("Content-Length", size_t_to_string(calc_input_stream_size(*temp_stream)));
		this->_response.set_body_stream(*temp_stream);
		this->_content = static_cast<std::istream *>(temp_stream);
	}
	this->_response.set_status_code(200);
}

void Client::_handle_post(const std::string &dir_path)
{
	if (this->_request.get_target().length() > 0)
	{
		throw HTTPException(501, "Not implemented");
	}
	// std::string							content_type_line;
	// std::string							content_type;
	// std::string							boundary_key_value_string;
	// std::pair<std::string, std::string>	boundary_key_value_pair;

	// content_type_line = this->_request.get_header("Content-Type");
	// trim_str(content_type_line, " \t\r\n");
	// content_type = content_type_line.substr(0, content_type_line.find_first_of(';'))
	// if (content_type != "multipart/form-data");
	// {
	// 	throw HTTPException(501, "Unsupported content-type");
	// }
	// boundary_key_value_string = trim_str(content_type_line.substr(content_type_line.find_first_of(';') + 1), " \t");
	// try
	// {	
	// 	boundary_key_value_pair = extract_key_value_pair(boundary_key_value_string, '=');
	// 	boundary_key_value_pair.second = trim_str(boundary_key_value_pair.second, "\"");
	// }
	// catch(const std::exception& e)
	// {
	// 	throw HTTPException(400, "Malformed request");;
	// }
	// if (boundary_key_value_pair.first != 'boundary')
	// {
	// 	throw HTTPException(400, "Malformed request");
	// }


	// std::ofstream outfile;

	// outfile.open((dir_path));
	this->_response.set_status_code(201);
}

void Client::_handle_delete(std::string file_path)
{
	if (file_path[file_path.length() - 1] == '/')
	{
		file_path.erase(file_path.length() - 1);
	}
	std::cout << "File to delete: " << file_path << std::endl;
	// if (remove(file_path.c_str()) == -1)
	// {
	// 	throw HTTPException(400, "File not found");
	// }
	this->_response.set_status_code(204);
}

void Client::_handle_exception()
{
	std::string								file_name = "";
	std::vector<std::vector<std::string> >	error_pages;
	std::ostringstream						oss;
	std::ifstream							*temp_stream = new std::ifstream;

	oss << this->_response.get_status_code();
	error_pages = this->_common_server_config->error_page();
	for (std::vector<std::vector<std::string> >::const_iterator cit = error_pages.begin(); cit != error_pages.end(); cit++)
	{
		for (std::size_t index = 0; index < (cit->size() - 1); index++)
		{
			if (cit->at(index) == oss.str())
			{
				file_name = cit->at(index);
				break;
			}
		}
		if (!file_name.empty())
			break;
	}
	if (file_name.empty())
		return;
	temp_stream->open((ERROR_PAGE_DIR + file_name).c_str());
	if (!temp_stream->is_open())
	{
		std::runtime_error("Couldn't open exception file");
	}

	std::string file_extension;
	std::map<std::string, std::string>::const_iterator mime_type_iterator;

	file_extension = extract_file_extension(file_name);
	if ((mime_type_iterator = Client::mime_type_map.find(file_extension)) != Client::mime_type_map.end())
	{
		this->_response.set_header("Content-Type", mime_type_iterator->second);
	}
	this->_response.set_body_stream(*temp_stream);
	this->_content = static_cast<std::istream *>(temp_stream);
}

void Client::_handle_redirect()
{
	std::stringstream	temp_stream;
	std::size_t			status_code;
	temp_stream << this->_common_server_config->redirect().at(0);
	temp_stream >> status_code;
	this->_response.set_header("Location", this->_common_server_config->redirect().at(1));
	this->_response.set_status_code(status_code);
}

char **Client::_prepare_cgi_meta_variables()
{
	std::vector<std::string> meta_variables_container;
	std::size_t i = 0;
	char **meta_variables = new char *[meta_variables_container.size() + 1];

	for (std::vector<std::string>::iterator it = meta_variables_container.begin(); it != meta_variables_container.end(); it++)
	{
		meta_variables[i++] = const_cast<char *>(it->c_str());
	}
	meta_variables[meta_variables_container.size()] = NULL;
	return meta_variables;
}

void Client::_configure_common_config()
{
	if (this->_endpoint.empty())
	{
		this->_common_server_config = static_cast<const CommonServerConfig *>(this->_server_config);
	}
	else
	{
		this->_common_server_config = static_cast<const CommonServerConfig *>(&this->_server_config->locations().find(this->_endpoint)->second);
	}
}

// std::string Client::_translate_binary_path()
// {
// 	// std::string	;

// 	return ;
// }

bool Client::_is_target_cgi()
{
	std::string file_extension;

	file_extension = extract_file_extension(this->_request.get_target());

	for (std::vector<std::vector<std::string> >::const_iterator it = this->_common_server_config->cgi().begin(); it != this->_common_server_config->cgi().end(); it++)
	{
		if (file_extension == (*it)[0])
		{
			return true;
		}
	}
	return false;
}

std::string Client::_get_cgi_binary()
{
	std::string file_extension;

	file_extension = extract_file_extension(this->_request.get_target());

	for (std::vector<std::vector<std::string> >::const_iterator it = this->_common_server_config->cgi().begin(); it != this->_common_server_config->cgi().end(); it++)
	{
		if (file_extension == (*it)[0])
		{
			return (*it)[1];
		}
	}
	return this->_request.get_target();
}

void Client::_initialize_cgi()
{
	std::string	binary_path;

	binary_path = this->_translate_binary_path();

	this->_cgi = new CGI(binary_path);
	for (std::map<std::string, std::string>::iterator it = this->_request.get_headers().begin(); it != this->_request.get_headers().end(); it++)
	{
		this->_cgi->add_envp(it->first, it->second);
	}
	this->_cgi->execute();
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

ServerConfig const* Client::get_server_config() const
{
	return this->_server_config;
}

#pragma endregion Getters

#pragma region Setters

void Client::set_server_config(ServerConfig *server_config)
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
	char buffer[this->_buffer_size + 1] = {0};
	std::size_t	bytes_read = 0;

	if ((bytes_read = recv(this->_fd, buffer, BUFFER_SIZE, MSG_NOSIGNAL)) <= 0)
	{
		return bytes_read;
	}
	this->_request.append_to_request(buffer);
	try
	{
		if (this->_request.get_body_size() > this->_common_server_config->client_max_body_size())
			throw HTTPException(413, "Request entity too large");
		this->_request.process_request();
	}
	catch(const HTTPException& e)
	{
		this->_response.set_status_code(e.get_status_code());
	}
	if (this->_request.get_request_read_state() == Request::FINISHED || this->_response.get_status_code() > 0)
	{
		this->_state = VALIDATING_REQUEST;
	}
	return bytes_read;
}

void Client::handle_response()
{
	try
	{
		if (this->_state == VALIDATING_REQUEST)
		{
			this->_match_location(this->_request.get_uri());
			this->_configure_common_config();
			this->_is_method_allowed();
			this->_state = PROCESSING_RESPONSE;
		}

		if (this->_state == PROCESSING_RESPONSE)
		{
			if (this->_is_target_cgi())
			{
				this->_initialize_cgi();
				this->_cgi->execute();
				this->_state = PROCESSING_CGI;
			}
			else if (this->_common_server_config->redirect().size() > 0)
			{
				this->_handle_redirect();
				this->_state == SENDING_RESPONSE;
			}
			else
			{
				this->_handle_method(this->_endpoint);
				this->_state == SENDING_RESPONSE;
			}
		}

		if (this->_state == PROCESSING_CGI)
		{
			int status;
			
			if (this->_cgi->get_state() == CGI::PROCESSING)
			{
				try
				{
					this->_cgi->check_completed();
				}
				catch (std::exception const& e)
				{
					throw HTTPException(500, "CGI error");
				}
			}

			struct timeval  timeout = {.tv_sec = 0, .tv_usec = 0};
			int read_fd = this->_cgi->get_read_fd();
			int	write_fd = this->_cgi->get_write_fd();
			int max_fd = (read_fd > write_fd) ? read_fd : write_fd;
			fd_set write_set;
			fd_set read_set;
			int activity;

			FD_ZERO(&read_set);
			FD_ZERO(&write_set);

			if (read_fd > STDERR_FILENO)
			{
				FD_SET(read_fd, &read_set);
			}

			if (write_fd > STDERR_FILENO)
			{
				FD_SET(write_fd, &write_set);
			}

			if ((activity = select(max_fd + 1, &read_set, &write_set, NULL, &timeout)) == -1 && errno != EINTR)
			{
				throw HTTPException(500, "CGI error");
			}

			if (FD_ISSET(write_fd, &write_set))
			{
				char *buffer;

				if (this->_request.get_body_stream().read(buffer, CGI_READ_BUFFER_SIZE))
				{
					this->_cgi->write_to_cgi(buffer);
					this->_cgi->update_time_since_last_activity(time(NULL));
				}
				else
				{
					this->_cgi->close_write_fd();
				}
			}

			if (FD_ISSET(read_fd, &read_set))
			{
				this->_cgi->read_cgi_stream(CGI_READ_BUFFER_SIZE);
				this->_cgi->update_time_since_last_activity(time(NULL));
			}

			if ((time(NULL) - this->_cgi->get_time_since_last_activity()) > CGI_TIMEOUT)
			{
				delete this->_cgi;
				this->_cgi = NULL;
				throw HTTPException(500, "CGI error");
			}

			if (this->_cgi->get_state() == CGI::FINISHED)
			{
				try
				{
					this->_cgi->process_output();
				}
				catch (const std::exception &e)
				{
					throw HTTPException(500, "Some kind of cgi error");
				}
				this->_response.set_header(this->_cgi->get_headers());
				this->_response.set_body_stream(this->_cgi->get_body());
				this->_state == SENDING_RESPONSE;
			}
		}
	}
	catch (const HTTPException &e)
	{
		this->_response.set_status_code(e.get_status_code());
		this->_state = PROCESSING_EXCEPTION;
	}
	
	if (this->_state == PROCESSING_EXCEPTION)
	{
		this->_handle_exception();
		this->_state == SENDING_RESPONSE;
	}
	if (this->_state == SENDING_RESPONSE && this->_response.get_message_format() == NULL)
	{
		this->_response.set_header("Connection", "Closed");
		this->_response.construct_response_message_format();
	}

	if (this->_state == SENDING_RESPONSE)
	{
		std::string read_response_str = this->_response.read_response(this->_buffer_size);
		send(this->_fd, read_response_str.c_str(), read_response_str.length(), MSG_NOSIGNAL);
	}
	if (this->_response.get_write_state() == Response::FINISHED)
	{
		this->_state = FINISHED_PROCESSING;
	}
}

#pragma endregion PublicMemberMethods

}
