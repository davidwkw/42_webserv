#include "Request.hpp"

namespace ft
{

Request::Request() : _request_start_line(), _headers(), _buffer_stream(), _body(), _header_break_index(std::string::npos){}

Request::~Request(){}

void Request::construct_message_format(const std::string &message_format_string)
{
	std::stringstream ss(message_format_string);

	this->_parse_request_line(ss);
	this->_parse_request_headers(ss);
	if (this->_headers.find("Host") == this->_headers.end())
	{
		throw HTTPException(400 , "Host field not found.");
	}
}

std::string Request::_validate_method(const std::string &method)
{
	// abit wonky innit?
	const std::string valid_methods[8] = {
		"OPTIONS",
		"GET",
		"HEAD",
		"POST",
		"PUT",
		"DELETE",
		"TRACE",
		"CONNECT"
	};

	for (int i = 0; i < 8; i++)
	{
		if (method.compare(valid_methods[i]) == 0)
		{
			return method;
		}
	}

	throw HTTPException(400, "Invalid method");
	return method;
}

std::string	Request::_extract_target(const std::string &uri_string)
{
	return uri_string.substr(uri_string.find_last_of('/') + 1, uri_string.find('?'));
}

// TO DO: double check if indexing is correct..
std::map<std::string, std::string> Request::_extract_query(const std::string &uri_string) // refactor this
{
	std::map<std::string, std::string> return_map;
	std::size_t query_begin_index;
	std::size_t next_index;
	std::pair<std::string, std::string> key_value_pair;

	query_begin_index = uri_string.find('?');
	while (query_begin_index != std::string::npos)
	{
		next_index = uri_string.find('&', query_begin_index + 1);
		key_value_pair = extract_key_value_pair(uri_string.substr(query_begin_index + 1, next_index), '=');
		query_begin_index = next_index;
		if (key_value_pair.first == "")
		{
			continue;
		}
		return_map.insert(key_value_pair);
	}
	return return_map;
}

std::string	Request::_extract_target_file(const std::string &uri_string)
{
	int			count;
	std::size_t	dot_pos;
	std::size_t	leading_backslash_pos;
	std::size_t	trailing_backslash_pos;
	std::string	target_file_name = "";

	count = std::count(uri_string.begin(), uri_string.end(), '.');
	if (count > 1)
	{
		throw HTTPException(404, "Invalid url");
	}
	if (count == 0)
	{
		return target_file_name;
	}

	dot_pos = uri_string.find_first_of('.');
	leading_backslash_pos = uri_string.find_last_of('/', dot_pos);
	trailing_backslash_pos = uri_string.find_first_of('/', dot_pos);
	target_file_name = uri_string.substr(leading_backslash_pos + 1, trailing_backslash_pos);

	return target_file_name;
}

void Request::_parse_request_line(std::stringstream &ss)
{
	std::vector<std::string>	request_line_tokens;
	std::string					line;

	getline_CRLF(ss, line);
	request_line_tokens = tokenise_str(line);
	if (request_line_tokens.size() != 3)
	{
		throw HTTPException(400, "Invalid request start line");
	}
	this->_request_start_line.construct(line);
}

void Request::_validate_header_field(const std::pair<std::string, std::string> &header_pair)
{
	if (header_pair.first == "")
	{
		throw HTTPException(400, "Missing header key");
	}
	if (header_pair.first == "Content-Length")
	{
		char		*end;
		long int	content_length = std::strtol(header_pair.second.c_str(), &end, 10);

		if (content_length < 0)
		{
			throw HTTPException(400, "Invalid Content-Length value");
		}
		else if (content_length == 0 && std::strcmp(end, "\0") != 0)
		{
			throw HTTPException(400, "Invalid conversion");
		}
	}
}

void Request::_handle_duplicate_headers(const std::string &key, const std::string &original_value, const std::string &duplicate_value)
{
	if (key == "Content-Length")
	{
		if (original_value == duplicate_value)
		{
			return;
		}
		else
		{
			throw HTTPException(400, "Multiple content-length headers");
		}
	}
}

void Request::_parse_request_headers(std::stringstream &ss)
{
	std::string	line;
	std::size_t separator_index;
	std::size_t whitespace_index;
	std::pair<std::string, std::string> header_field_pair;
	std::pair<std::map<std::string, std::string>::iterator,bool> insert_return;

	while (getline_CRLF(ss, line) && !line.empty())
	{
		separator_index = line.find(':');
		whitespace_index = line.find_first_of(WHITESPACE_CHARACTERS);
		if (whitespace_index < separator_index)
		{
			throw HTTPException(400, "Whitespace found before : during header parsing");
		}
		header_field_pair = extract_key_value_pair(line, ':');
		this->_validate_header_field(header_field_pair);
		if (header_field_pair.first == "Host")
		{
			header_field_pair.second = prune_http_protocol(header_field_pair.second);
			header_field_pair.second = url_decode(header_field_pair.second);
		}
		insert_return = this->_headers.insert(header_field_pair);
		if (insert_return.second == false)
		{
			this->_handle_duplicate_headers((*insert_return.first).first, (*insert_return.first).second, header_field_pair.second);
		}
    }
}

std::string Request::_parse_chunked_request_body()
{
	std::ostringstream 	oss;
    std::string			line;
    std::string			chunk_size_str;
    std::size_t			chunk_size;

	while (getline_CRLF(this->_buffer_stream, line) && !line.empty())
	{
        std::istringstream chunk_line_stream(line);
        chunk_line_stream >> std::hex >> chunk_size_str;

        if (chunk_line_stream.fail())
		{
            std::cerr << "Invalid chunk size format: " << line << std::endl;
            throw HTTPException(400, "Invalid chunk size format");
        }

        chunk_size = hex_str_to_ulong(chunk_size_str);

        if (chunk_size == 0)
		{
            break;
        }

        std::vector<char> chunk_data(chunk_size);
        this->_buffer_stream.read(chunk_data.data(), chunk_size);

		if (chunk_size != chunk_data.size())
		{
			throw HTTPException(400, "Chunk size and chunk data mismatch");
		}
	
        oss.write(chunk_data.data(), chunk_size);
		getline_CRLF(this->_buffer_stream, line);
    }
	return oss.str();
}

std::string Request::_parse_encoded_request_body()
{
	return this->_parse_chunked_request_body();
	// this->_parse_request_headers();
}

std::string Request::_identify_content_type()
{
	std::string										content_type_string;

	try
	{
		content_type_string = this->_headers.at("Content-Type");
		content_type_string = content_type_string.substr(0, content_type_string.find(';'));
	}
	catch (const std::out_of_range &e){}
	return content_type_string;
}

std::string Request::_identify_boundary_string()
{
	std::map<std::string, std::string>::iterator	header_iterator;
	std::string										result;

	try
	{
		result = this->_headers.at("Content-Type");
		result = result.substr(result.find(';') + 1);
	}
	catch(const std::out_of_range& e){}
	return trim_chars(result, WHITESPACE_CHARACTERS);
}

std::string Request::get_method() const
{
	return this->_request_start_line.get_method();
}

std::string Request::get_target() const
{
	return this->_request_start_line.get_target();
}

std::string Request::get_target_file() const
{
	return this->_request_start_line.get_target_file();
}

std::string Request::get_protocol() const
{
	return this->_request_start_line.get_protocol();
}

std::size_t Request::get_body_size() const
{
	std::size_t body_size = 0;
	std::size_t header_break_index = this->_buffer_stream.str().find(CRLF CRLF);

	if (header_break_index == std::string::npos)
	{
		return body_size;
	}
	body_size = this->_buffer_stream.str().length() - body_size + 5;
	return body_size;
}

std::string Request::get_header(const std::string &key) const
{
	std::string		value;
	
	try
	{
		value = this->_headers.at(key);
	}
	catch (const std::out_of_range &e)
	{
		value = "";
	}
	return value;
}

std::map<std::string, std::string> Request::get_headers() const
{
	return this->_headers;
}

std::stringstream &Request::get_raw_body_stream()
{
	return this->_body->get_raw_stream();
}

std::string Request::get_raw_body_string() const
{
	return this->_body->get_raw_string();
}

RequestBody	&Request::get_body() const
{
	return *this->_body;
}

std::string Request::get_query_string() const
{
	return this->_request_start_line.get_query_string();
}

std::string Request::get_query_param(const std::string key) const
{
	std::string value;
	
	try
	{
		value = this->_request_start_line.get_query_map().at(key);
	}
	catch (const std::out_of_range &e)
	{
		value = "";
	}
	return value;
}

bool Request::has_body_headers() const
{
	return this->_headers.find("Transfer-Encoding") != this->_headers.end() || this->_headers.find("Content-Length") != this->_headers.end();
}

void Request::set_body(RequestBody *request_body_ptr)
{
	this->_body.reset(request_body_ptr);
}

}
