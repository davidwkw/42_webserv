#include "Client.hpp"
#include <unistd.h>

extern char **environ;

namespace ft
{

const std::map<std::string, std::string> Client::mime_type_map = Client::_fill_mime_type_map();

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

Client::Client(int fd, std::size_t buffer_size) : _state(READING_REQUEST_HEADERS), _fd(fd), _buffer_stream(), _common_server_config(NULL), _request(), _response(HTTP_PROTOCOL),_buffer_size(buffer_size), _endpoint(), _dir_path(), _cgi(NULL), _client_port(), _client_ip(), _server_port(), _server_ip()
{
	struct sockaddr_in	client_addr = {};
	socklen_t			client_addr_len = sizeof(client_addr);
	char 				client_ip[INET_ADDRSTRLEN];

	if (getpeername(this->_fd, (struct sockaddr *)&client_addr, &client_addr_len) == -1)
	{
		throw std::runtime_error("Error with getpeername()");
	}
	if (inet_ntop(AF_INET, &client_addr, client_ip, INET_ADDRSTRLEN) == NULL)
	{
		throw std::runtime_error("Error with client inet_ntop()");
	}
	this->_client_port = ntohs(client_addr.sin_port);
	this->_client_ip = client_ip;

	struct sockaddr_in	server_addr = {};
	socklen_t			server_addr_len = sizeof(server_addr);
	char server_ip[INET_ADDRSTRLEN];

	if (getsockname(this->_fd, (struct sockaddr *)&server_addr, &server_addr_len) == -1)
	{
		throw std::runtime_error("Error with getsockname()");
	}

	if (inet_ntop(AF_INET, &server_addr, server_ip, INET_ADDRSTRLEN) == NULL)
	{
		throw std::runtime_error("Error with server inet_ntop()");
	}

	this->_server_port = ntohs(server_addr.sin_port);
	this->_server_ip = server_ip;
}

Client::~Client(){}

int	Client::get_fd() const
{
	return this->_fd;
}

Request const &Client::get_request() const
{
	return this->_request;
}

Request &Client::get_request()
{
	return this->_request;
}

Response &Client::get_response()
{
	return this->_response;
}

Client::ClientState Client::get_client_state() const
{
	return this->_state;
}

CommonServerConfig const &Client::get_common_server_config() const
{
	return *(this->_common_server_config.get());
}

std::string	Client::get_server_ip() const
{
	return this->_server_ip;
}

std::string	Client::get_client_ip() const
{
	return this->_client_ip;
}

int	Client::get_server_port() const
{
	return this->_server_port;
}

int	Client::get_client_port() const
{
	return this->_client_port;
}

void Client::set_server_config(ServerConfig *server_config)
{
	this->_common_server_config.reset(static_cast<CommonServerConfig *>(server_config));
}

void Client::set_client_state(const ClientState &state)
{
	this->_state = state;
}

std::size_t Client::read_to_buffer()
{
	std::auto_ptr<char>	buffer(new char[this->_buffer_size]);
	std::size_t			bytes_read;

	std::memset(buffer.get(), 0, this->_buffer_size);
	if ((bytes_read = recv(this->_fd, buffer.get(), this->_buffer_size, MSG_NOSIGNAL)) <= 0)
	{
		std::cerr << "failed recv" << std::endl;
		return bytes_read;
	}
	this->_buffer_stream << std::string(buffer.get());
	return bytes_read;
}

void Client::handle_request_header_construction()
{
	try
	{
		if (!this->_has_full_message_format_received())
		{
			return;
		}
		this->_request.construct_message_format(this->_buffer_stream.str().substr(0, this->_buffer_stream.str().find(CRLF CRLF) + 2));
		this->_state = REQUEST_HEADERS_CONSTRUCTED;
	}
	catch(const HTTPException& e)
	{
		this->_response.set_status_code(e.get_status_code());
		this->_state = PROCESSING_EXCEPTION;
	}
}

void Client::check_request_body()
{
	try
	{
		if (this->_request.get_method() == "POST" && this->_request.has_body_headers())
		{
			this->_check_request_body_headers();	
			this->_buffer_stream.clear();
			this->_buffer_stream.str(this->_buffer_stream.str().substr(this->_buffer_stream.str().find(CRLF CRLF) + 4));
			this->_buffer_stream.seekg(0);
			this->_buffer_stream.seekp(this->_buffer_stream.str().length());
			this->_state = READING_REQUEST_BODY;
		}
		else
		{
			this->_buffer_stream.str("");
			this->_state = PROCESSING_RESPONSE;
		}
	}
	catch(const HTTPException& e)
	{
		this->_response.set_status_code(e.get_status_code());
		this->_buffer_stream.str("");
		this->_state = PROCESSING_EXCEPTION;
	}
}

void Client::handle_request_body()
{
	try
	{
		if (!this->_request.get_header("Transfer-Encoding").empty())
		{
			this->_parse_chunked_request_body(this->_buffer_stream);
		}
		this->_check_body_size_within_limits(); // for chunked encoded, size checking will be inaccurate up to 9 + the length of the chunk-size line; would consider as a better than the alternative where a user can greatly exceed max size
		
		if (!this->_has_received_complete_body())
		{
			std::cerr << "not yet received full body" << std::endl;
			return;	
		}
		std::cerr << "received full body" << std::endl;
		this->_create_request_body();
		this->_buffer_stream.str("");
		this->_state = PROCESSING_RESPONSE;
	}
	catch(const HTTPException& e)
	{
		this->_response.set_status_code(e.get_status_code());
		this->_buffer_stream.str("");
		this->_state = PROCESSING_EXCEPTION;
	}
}

void Client::handle_response()
{
	try
	{
		if (this->_state == PROCESSING_RESPONSE)
		{
			if (!this->_request.get_target_file().empty() && this->_is_target_cgi()) // if there was a file destination and that file is a cgi script
			{
				this->_initialize_cgi();
				this->_cgi->execute();
				this->_state = PROCESSING_CGI;
			}
			else if (this->_common_server_config->redirect().size() > 0)
			{
				this->_handle_redirect();
				this->_state = SENDING_RESPONSE;
			}
			else
			{
				this->_handle_method();
				this->_state = SENDING_RESPONSE;
			}
		}

		if (this->_state == PROCESSING_CGI)
		{			
			if (this->_cgi->get_state() == CGI::PROCESSING)
			{
				try
				{
					this->_cgi->update_state();
				}
				catch (const std::exception &e)
				{
					throw HTTPException(500, "CGI error");
				}
			}

			struct timeval  timeout = {};
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
				std::auto_ptr<char> buffer(new char[CGI_READ_BUFFER_SIZE]);

				std::memset(buffer.get(), 0, CGI_READ_BUFFER_SIZE);
				if (&this->_request.get_body() != NULL
					&& this->_request.get_raw_body_stream().read(buffer.get(), CGI_READ_BUFFER_SIZE))
				{
					this->_cgi->write_to_cgi(buffer.get());
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
			
			// only check timeout if still processing
			if (this->_cgi->get_state() == CGI::PROCESSING && (std::difftime(time(NULL), this->_cgi->get_time_since_last_activity())) > CGI_TIMEOUT)
			{
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
				this->_response.append_headers(this->_cgi->get_headers());
				this->_response.set_body_stream(static_cast<std::istream *>(new std::stringstream(this->_cgi->get_body_string())));
				this->_response.set_status_code(this->_cgi->get_response_status());
				this->_state = SENDING_RESPONSE;
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
		this->_state = SENDING_RESPONSE;
	}

	if (this->_state == SENDING_RESPONSE && this->_response.get_message_format() == NULL)
	{
		this->_response.set_header("Connection", "close");
		this->_response.set_header("Server", SERVER_NAME);
		if (this->_response.get_body_stream() != NULL)
		{
			this->_response.set_header("Content-Length", to_string(calc_input_stream_size(*this->_response.get_body_stream())));
		}
		this->_response.construct_response_message_format();
	}

	if (this->_state == SENDING_RESPONSE)
	{
		int			sent_length = 0;
		int			length_to_send = 0;
		std::string read_response_str;

		read_response_str = this->_response.read_response(this->_buffer_size);
		length_to_send = (read_response_str.length() < this->_buffer_size) ? read_response_str.length() : this->_buffer_size;
		sent_length = send(this->_fd, read_response_str.c_str(), length_to_send, MSG_NOSIGNAL);
		if (sent_length == -1)
		{
			this->_state = FINISHED_PROCESSING;
		}
		else if (sent_length < static_cast<int>(length_to_send)) // todo: somethng wrong here
		{
			this->_response.unread_response(length_to_send - sent_length);
		}
	}
	if (this->_state == SENDING_RESPONSE
		&& this->_response.has_been_completely_read())
	{
		this->_state = FINISHED_PROCESSING;
	}
}

void Client::process_header()
{
	this->_match_location();
	this->_configure_common_config();
	this->_is_method_allowed();
	this->_dir_path = this->_common_server_config->root() + (this->_endpoint.empty() ? "/" : this->_endpoint);
}

std::size_t Client::_get_body_size()
{
	return this->_buffer_stream.str().length();
}

void Client::_check_body_size_within_limits()
{
	if (this->_get_body_size() > this->_common_server_config->client_max_body_size())
	{
		throw HTTPException(413, "Request entity too large");
	}
}

bool Client::_has_full_message_format_received()
{
	if (this->_state == READING_REQUEST_HEADERS)
	{
		return this->_buffer_stream.str().find(CRLF CRLF) != std::string::npos;
	}
	return true;
}

bool Client::_has_received_complete_body()
{
	if (!this->_request.get_header("Transfer-Encoding").empty())
	{
		if (this->_buffer_stream.str().rfind("0" CRLF CRLF) != std::string::npos)
		{
			return true;
		}
	}
	else if (!this->_request.get_header("Content-Length").empty())
	{
		std::cerr << "current length: " <<  this->_get_body_size() << std::endl;
		std::string		content_length;
		unsigned long	content_length_value;

		content_length = this->_request.get_header("Content-Length");
		content_length_value = std::strtoul(content_length.c_str(), NULL, 10);
		if (this->_get_body_size() >= content_length_value)
		{
			return true;
		}
	}
	return false;
}

std::string Client::_identify_boundary_string()
{
	std::string							result;
	std::pair<std::string, std::string>	kv_pair;
	std::string							boundary_line;

	result = this->_request.get_header("Content-Type");
	if (result.empty()
		|| this->_identify_content_type() != "multipart/form-data")
	{
		return result;
	}
	boundary_line = trim_chars(result.substr(result.find(';') + 1), WHITESPACE_CHARACTERS);
	kv_pair = extract_key_value_pair(boundary_line, '=');
	if (trim_chars(kv_pair.first, WHITESPACE_CHARACTERS) != "boundary")
	{
		return result;
	}
	return trim_chars(kv_pair.second, "\"");
}

void Client::_check_content_length_header_within_limits()
{
	std::string		content_length_header_value;
	unsigned long	content_length_value_from_header;

	content_length_header_value = this->_request.get_header("Content-Length");
	content_length_value_from_header = std::strtoul(content_length_header_value.c_str(), NULL, 10);
	if (content_length_value_from_header > this->_common_server_config->client_max_body_size())
	{
		throw HTTPException(413, "Request entity too large");
	}
}

void Client::_create_request_body()
{
	std::string	content_type;
	std::string parsed_body_string;

	if (!this->_request.get_header("Transfer-Encoding").empty())
	{
		this->_buffer_stream.str(this->_buffer_stream.str().substr(0, this->_buffer_stream.str().rfind("0" CRLF CRLF)));
	}
	else if (!this->_request.get_header("Content-Length").empty())
	{
		unsigned long content_length_value;

		content_length_value = std::strtoul(this->_request.get_header("Content-Length").c_str(), NULL, 10);
		this->_buffer_stream.str(this->_buffer_stream.str().substr(0, content_length_value));
	}

	content_type = this->_identify_content_type();
	if (content_type == "multipart/form-data")
	{
		std::string boundary_string;

		boundary_string = this->_identify_boundary_string();
		this->_request.set_body(RequestBodyFactory(this->_buffer_stream.str()).build_multipart(boundary_string));
	}
	else if (content_type == "application/x-www-form-urlencoded")
	{
		this->_request.set_body(RequestBodyFactory(this->_buffer_stream.str()).build_form_encoded());
	}
}

void Client::_parse_chunked_request_body(std::stringstream &stream)
{
	std::ostringstream 	oss;
    std::string			line;
    std::string			chunk_size_str;
    std::size_t			chunk_size;
    std::streampos      previous_read_pos;
    std::streampos      original_pos;

    previous_read_pos = stream.tellg();
    original_pos = previous_read_pos;
	while (getline_CRLF(stream, line) && !line.empty())
	{
        std::istringstream  chunk_line_stream(line.substr(0, line.find_first_of(';')));

        chunk_line_stream >> std::hex >> chunk_size;
        
        if (chunk_line_stream.fail())
		{
            throw HTTPException(400, "Invalid chunk size format");
        }

        if (chunk_size == 0)
		{
            break;
        }
        
		getline_CRLF(stream, line);
		
		if (stream.eof()) // premature eof when reading chunk body
		{
		    stream.clear();
		    stream.seekg(previous_read_pos);
		    break;
		}

		if (chunk_size != line.length())
		{
			throw HTTPException(400, "Chunk size and chunk data mismatch");
		}
        oss << line;
        previous_read_pos = stream.tellg();
    }

	if (stream.eof()) // only triggers with incomplete chunk data size
	{
		stream.clear();
	}
	
	stream.str(stream.str().replace(original_pos, previous_read_pos - original_pos, oss.str()));
	stream.seekg(static_cast<std::size_t>(original_pos) + oss.str().length()); // problem here
	stream.seekp(stream.str().length());
}

std::string Client::_identify_content_type()
{
	std::map<std::string, std::string>::iterator	header_iterator;
	std::string										content_type_string;

	content_type_string = this->_request.get_header("Content-Type");
	if (content_type_string.empty())
	{
		return content_type_string;
	}
	content_type_string = content_type_string.substr(0, content_type_string.find(';'));
	return trim_chars(content_type_string, WHITESPACE_CHARACTERS);
}

void Client::_check_request_body_headers()
{
	if (!this->_request.get_header("Content-Length").empty())
	{
		this->_check_content_length_header_within_limits();
	}

	{
		std::string content_type;

		content_type = this->_identify_content_type();
		if (content_type.empty() 
			|| (content_type != "multipart/form-data"
				&& content_type != "application/x-www-form-urlencoded"))
		{
			throw HTTPException(400, "Unsupported content-type");
		}
	}

	{
		std::string transfer_encoding;

		transfer_encoding = this->_request.get_header("Transfer-Encoding");
		if (!transfer_encoding.empty())
		{
			std::vector<std::string> temp = tokenise_str(transfer_encoding);

			if (trim_chars(temp[temp.size() - 1], std::string(WHITESPACE_CHARACTERS) + ",") != "chunked")
			{
				throw HTTPException(400, "Chunked was not the last encoding type.");
			}
		}
	}

}

void Client::_handle_method()
{
	if (this->_request.get_method() == "GET")
	{
		this->_handle_get(this->_dir_path + this->_request.get_target_file());
	}
	else if (this->_request.get_method() == "POST")
	{
		this->_handle_post(this->_dir_path);
	}
	else if (this->_request.get_method() == "DELETE")
	{
		this->_handle_delete(this->_dir_path + this->_request.get_target_file());
	}
}

void Client::_match_location()
{
	std::string	target_str;
	int			number_of_extensions;
	std::string endpoint_str;
	std::size_t	extension_pos;
	std::size_t slash_index;

	target_str = url_decode(this->_request.get_target());
	target_str = target_str.substr(0, target_str.find_first_of('?'));
	number_of_extensions = std::count(target_str.begin(), target_str.end(), '.');
	if (number_of_extensions > 1)
	{
		throw HTTPException(404, "Invalid path");
	}
	extension_pos = target_str.find_first_of('.');
	slash_index = target_str.find_last_of('/', extension_pos);
	endpoint_str = target_str;
	while (slash_index != std::string::npos)
	{
		endpoint_str = endpoint_str.substr(0, slash_index + 1);
		std::map<std::string, LocationConfig> locations;
		ServerConfig *server_config;

		server_config = dynamic_cast<ServerConfig *>(this->_common_server_config.get());
		locations = server_config->locations();
		if (locations.find(endpoint_str) != locations.end())
		{
			this->_endpoint = endpoint_str;
			return;
		}
		if (slash_index == 0)
		{
			break;
		}
		slash_index = endpoint_str.find_last_of('/', slash_index - 1);
	}
	this->_endpoint = "";
}

void Client::_is_method_allowed()
{
	std::set<std::string> limit_except;

	limit_except = this->_common_server_config->limit_except();
	if (limit_except.find(this->_request.get_method()) == limit_except.end())
	{
		throw HTTPException(405, "Method not allowed");
	}
}

std::string Client::_generate_dir_content_list_html(const std::string &dir_path)
{
	std::ostringstream	oss;
    struct dirent		*entry;
	DIR					*folder;

    folder = opendir(dir_path.c_str());
    if (folder == NULL)
	{
        throw HTTPException(500, "Couldn't open dir " + dir_path);
	}
	std::string root_str;
	std::string	end_path;
	
	root_str = this->_common_server_config->root();
	end_path = dir_path;
	end_path = end_path.erase(end_path.find(root_str), root_str.length());
	while ((entry = readdir(folder)) != NULL)
	{
		if (entry->d_type == DT_REG)
		{
			std::string	link;

			link = this->_request.get_header("Host") + end_path + std::string(entry->d_name);
			oss << "<li>";
			oss << "<a href=\"http://" << link.c_str() << "\">" << entry->d_name << "</a>\n";
			oss << "</li>";
		}
	}
	closedir(folder);
	return oss.str();
}

void Client::_handle_autoindex(const std::string &dir, std::stringstream &stream)
{
	std::ostringstream	temp_autoindex_path;
	std::ifstream		auto_index_template;
	std::string			line;
	std::string			content_path;
	std::string			root;

	root = this->_common_server_config->root();
	content_path = dir;
	content_path.erase(content_path.find(root), root.length());
	auto_index_template.open(AUTOINDEX_TEMPLATE_PATH);
	if (!auto_index_template.is_open())
	{
		throw HTTPException(500, "Critical mess up. Couldn't open autoindex.html");
	}
	while (std::getline(auto_index_template, line))
	{
		std::size_t identifier_pos = std::string::npos;

		while ((identifier_pos = line.find(DIRECTORY_TEMPLATE_IDENTIFIER)) != std::string::npos)
		{
			line.replace(identifier_pos, std::string(DIRECTORY_TEMPLATE_IDENTIFIER).length(), content_path);
		}
		while ((identifier_pos = line.find(DIRECTORY_CONTENT_TEMPLATE_IDENTIFIER)) != std::string::npos)
		{
			line.replace(identifier_pos, std::string(DIRECTORY_CONTENT_TEMPLATE_IDENTIFIER).length(), this->_generate_dir_content_list_html(dir));
		}
		stream << line << std::string(CRLF);
	}
}

void Client::_handle_get(const std::string &file_path)
{
	std::string											file_extension;
	std::map<std::string, std::string>::const_iterator	mime_type_iterator;

	if (!this->_request.get_target_file().empty()) // destination with no file, this is checked instead of file_path because file_path already has root and etc appended
	{
		std::ifstream *temp_stream = new std::ifstream;

		temp_stream->open(file_path.c_str());
		if (!temp_stream->is_open())
		{
			throw HTTPException(404, "File not found");
		}
		file_extension = extract_file_extension(this->_request.get_target_file());
		if ((mime_type_iterator = Client::mime_type_map.find(file_extension)) != Client::mime_type_map.end())
		{
			this->_response.set_header("Content-Type", mime_type_iterator->second);
		}
		this->_response.set_body_stream(temp_stream);
	}
	else if (this->_common_server_config->autoindex() == "on")
	{
		std::stringstream *temp_stream = new std::stringstream;

		this->_handle_autoindex(file_path, *temp_stream);
		this->_response.set_header("Content-Type", "text/html"); // had to hardcode..
		this->_response.set_body_stream(temp_stream);
	}
	else
	{
		const std::vector<std::string> indexs = this->_common_server_config->index();
		std::ifstream *temp_stream = new std::ifstream;

		for (std::vector<std::string>::const_iterator cit = indexs.begin(); cit != indexs.end(); cit++)
		{
			std::string const &index_file_name = *cit;

			temp_stream->open((file_path + index_file_name).c_str());
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
		this->_response.set_body_stream(temp_stream);
	}
	this->_response.set_status_code(200);
}

void Client::_handle_post(const std::string &dir_path)
{
	RequestBody &request_body = this->_request.get_body();

	if (request_body.get_type() == "multipart/form-data")
	{
		std::map<int, RequestMultipart>	index_multipart;
		std::vector<std::string>		files;

		index_multipart = request_body.get_index_multipart();
		try
		{
			for (std::map<int, RequestMultipart>::iterator it = index_multipart.begin(); it != index_multipart.end(); it++)
			{
				RequestMultipart	&file_part = it->second; // for readability;
				std::string			filename;
				std::size_t			forward_slash_pos;

				filename = file_part.get_filename();
				if (filename.length() == 0) // missing filename, happens with drag and drop file upload;
				{
					throw HTTPException(400, "Missing filename");
				}
				// prune filename to its terminal component for security
				forward_slash_pos = filename.rfind("/");
				if (forward_slash_pos != std::string::npos)
				{
					filename = filename.substr(forward_slash_pos + 1);
				}
				if (extract_file_extension(filename).length() == 0)
				{
					throw HTTPException(400, "Missing file extension");
				}
				if (!is_system_compatible_filename(filename))
				{
					throw HTTPException(400, "Filename incompatible with system");
				}
				
				std::ofstream	outfile;
				std::string		full_path;
				
				full_path = dir_path + filename;
				{
					std::ifstream filecheck;

					filecheck.open(full_path.c_str());
					if (filecheck.is_open())
					{
						throw HTTPException(400, "File already exists");
					}
				}
				outfile.open(full_path.c_str());
				if (!outfile.is_open())
				{
					throw HTTPException(500, "Error creating file");
				}
				outfile << file_part.get_content();
				outfile.close();
				files.push_back(full_path);
			}
		}
		catch (const HTTPException &e)
		{
			for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); it++)
			{
				remove(it->c_str());
			}
			throw e;
		}
	}
	this->_response.set_status_code(201);
}

void Client::_handle_delete(const std::string &file_path)
{
	if (this->_request.get_target_file().empty())
	{
		throw HTTPException(400, "Bad request");
	}
	if (remove(file_path.c_str()) == -1)
	{
		throw HTTPException(404, "File not found");
	}
	this->_response.set_status_code(204);
}

void Client::_handle_exception()
{
	std::string								file_name = "";
	std::vector<std::vector<std::string> >	error_pages;
	std::string								status_code;
	std::ifstream							*temp_stream = new std::ifstream;

	status_code = to_string(this->_response.get_status_code());
	error_pages = this->_common_server_config->error_page();
	for (std::vector<std::vector<std::string> >::const_iterator cit = error_pages.begin(); cit != error_pages.end(); cit++)
	{
		for (std::size_t index = 0; index < (cit->size() - 1); index++)
		{
			try
			{
				if (cit->at(index) == status_code)
				{
					file_name = cit->back();
					break;
				}
			}
			catch(const std::out_of_range& e)
			{
				continue;
			}
		}
	}
	if (file_name.empty())
	{
		file_name = std::string(DEFAULT_ERROR_PAGE_DIR) + status_code + ".html";
	}
	temp_stream->open((std::string(ERROR_PAGE_DIR) + file_name).c_str());
	if (!temp_stream->is_open())
	{
		file_name = std::string(ERROR_PAGE_DIR) + std::string(DEFAULT_ERROR_PAGE_DIR) + "500.html";
	}

	std::string file_extension;
	std::map<std::string, std::string>::const_iterator mime_type_iterator;

	file_extension = extract_file_extension(file_name);
	mime_type_iterator = Client::mime_type_map.find(file_extension);
	if (mime_type_iterator != Client::mime_type_map.end())
	{
		this->_response.set_header("Content-Type", mime_type_iterator->second);
	}
	this->_response.set_body_stream(temp_stream);
}

void Client::_handle_redirect()
{
	std::stringstream	temp_stream;
	std::size_t			status_code;

	try
	{
		temp_stream << this->_common_server_config->redirect().at(0);
		temp_stream >> status_code;
		this->_response.set_header("Location", this->_common_server_config->redirect().at(1));
		this->_response.set_status_code(status_code);
	}
	catch (const std::out_of_range &e)
	{
		throw HTTPException(500, "Misconfigured redirect");
	}
}

void Client::_configure_common_config()
{
	if (!this->_endpoint.empty())
	{
		try
		{
			std::map<std::string, LocationConfig>	locations;
			ServerConfig							*server_config;

			server_config = dynamic_cast<ServerConfig *>(this->_common_server_config.get());
			locations = server_config->locations();
			this->_common_server_config.reset(static_cast<CommonServerConfig *>(new LocationConfig(locations.at(this->_endpoint))));
		}
		catch (const std::out_of_range &e)
		{
			return;
		}
	}
}

std::string Client::_get_cgi_path_info()
{
	std::size_t target_pos;
	
	target_pos = this->_request.get_target().find(this->_request.get_target_file());
	return this->_request.get_target().substr(target_pos + this->_request.get_target_file().length());
}

std::string Client::_translate_binary_path()
{
	std::string file_extension;
	std::string	binary_path;
	std::size_t	relative_path_prefix_pos;

	file_extension = extract_file_extension(this->_request.get_target_file());
	binary_path = this->_extract_binary_path_from_config(file_extension);

	if (binary_path.empty()) // failed to match
	{
		throw HTTPException(404, "Couldn't find binary path"); // technically shouldn't be able to trigger
	}
	relative_path_prefix_pos = binary_path.find("./");
	if (relative_path_prefix_pos == std::string::npos) // found absolute path
	{
		return binary_path;
	}
	if (relative_path_prefix_pos > 0) // misplaced relative prefix
	{
		throw HTTPException(404, "Script not found");
	}
	if (binary_path.substr(2).empty()) // path of the file is itself?
	{
		binary_path = this->_dir_path + this->_request.get_target_file();
	}
	return binary_path;
}

std::string Client::_extract_binary_path_from_config(const std::string &file_extension)
{
	std::vector<std::vector<std::string> > cgis;

	cgis = this->_common_server_config->cgi();
	for (std::vector<std::vector<std::string> >::const_iterator it = cgis.begin(); it != cgis.end(); it++)
	{
		if (file_extension == (*it)[0])
		{
			return (*it)[1];
		}
	}
	return "";
}

bool Client::_is_target_cgi()
{
	std::string file_extension;
	std::string	binary_path;

	file_extension = extract_file_extension(this->_request.get_target_file());
	binary_path = _extract_binary_path_from_config(file_extension);

	return (!binary_path.empty());
}

void Client::_initialize_cgi()
{
	std::string					binary_path;
	std::vector<std::string>	args;

	binary_path = this->_translate_binary_path();
	this->_cgi.reset(new CGI());
	this->_cgi->set_binary(binary_path);
	if (binary_path.substr(binary_path.find_last_of('/') + 1) != this->_request.get_target_file())
	{
		this->_cgi->add_arg(this->_dir_path + this->_request.get_target_file());
	}

	for (char **envp = environ; *envp; envp++)
	{
		this->_cgi->add_server_envp(*envp);
	}

	this->_cgi->add_envp("PATH_INFO", this->_get_cgi_path_info());
	this->_cgi->add_envp("AUTH_TYPE", "");
	this->_cgi->add_envp("CONTENT_LENGTH", to_string(this->_request.get_body_size()));
	this->_cgi->add_envp("CONTENT_TYPE", this->_request.get_header("Content-Type"));
	this->_cgi->add_envp("GATEWAY_INTERFACE", "CGI/" + to_string(CGI_GATEWAY_INTERFACE_VERSION));
	this->_cgi->add_envp("QUERY_STRING", this->_request.get_query_string());
	this->_cgi->add_envp("REMOTE_ADDR", this->_client_ip);
	this->_cgi->add_envp("REMOTE_HOST", this->_request.get_header("Host"));
	this->_cgi->add_envp("REQUEST_METHOD", this->_request.get_method());
	this->_cgi->add_envp("SCRIPT_NAME", this->_request.get_target_file());
	this->_cgi->add_envp("SERVER_NAME", this->_request.get_header("Host"));
	this->_cgi->add_envp("SERVER_PORT", to_string(this->_server_port));
	this->_cgi->add_envp("SERVER_PROTOCOL", "http");
	this->_cgi->add_envp("SERVER_SOFTWARE", SERVER_NAME + to_string(SOFTWARE_VERSION));

	std::map<std::string, std::string> headers;

	headers = this->_request.get_headers();
	for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
	{
		if (it->first == "Content-Length" || it->first == "Content-Type")
		{
			continue;
		}
		this->_cgi->add_envp(it->first, "HTTP_" + it->second);
	}
}

}
