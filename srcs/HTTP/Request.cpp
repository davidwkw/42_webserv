#include "Request.hpp"

namespace ft
{

#pragma region Constructor

Request::Request() : _buffer_stream(), _request_start_line(), _headers(), _body(), _read_state(Request::READING){}

Request::Request(const std::string &request_string) : _buffer_stream(), _request_start_line(), _headers(), _body(), _read_state(Request::READING){}

Request::Request(const Request &ref) : _buffer_stream(ref._buffer_stream), _request_start_line(ref._request_start_line), _headers(ref._headers), _body(ref._body)
{
	*this = ref;
}

Request::~Request(){}

#pragma endregion Constructor

#pragma region OperatorOverload

Request &Request::operator=(const Request &ref)
{
	if (this != &ref)
	{
		this->_buffer_stream = ref._buffer_stream;
		this->_request_start_line = ref._request_start_line;
		this->_headers = ref._headers;
		this->_body = ref._body;
	}
	return *this;
}

#pragma endregion OperatorOverload

#pragma region ClassUtility

std::string Request::_validate_method(std::string method)
{
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
			return method;
	}
	throw HTTPException(400, "Invalid method");
}

std::string Request::_extract_uri(const std::string &uri_string)
{
	return uri_string.substr(0, uri_string.find_last_of('/') + 1);
}

std::string	_extract_target(const std::string &uri_string)
{
	return uri_string.substr(uri_string.find_last_of('/') + 1, uri_string.find('?'));
}

// TO DO: double check if indexing is correct..
std::map<std::string, std::string> Request::_extract_query(const std::string &uri_string)
{
	std::map<std::string, std::string> return_map;
	std::size_t query_begin_index;
	std::size_t next_index;

	query_begin_index = uri_string.find('?');
	while (query_begin_index != std::string::npos)
	{
		next_index = uri_string.find('&', query_begin_index + 1);
		try
		{
			return_map.insert(extract_key_value_pair(uri_string.substr(query_begin_index + 1, next_index), '='));
		}
		catch(const std::exception& e)
		{
			throw HTTPException(400, e.what());
		}
		query_begin_index = next_index;
	}
	return return_map;
}

void Request::_parse_request_line()
{
	std::vector<std::string> request_line_tokens;
	std::string line;

	ft::getline_CRLF(this->_buffer_stream, line);
	request_line_tokens = tokenise_str(line);
	if (request_line_tokens.size() != 3)
	{
		std::runtime_error("Invalid request start line");
	}
	std::string decoded_uri = url_decode(request_line_tokens[1]);

	this->_request_start_line.method = this->_validate_method(request_line_tokens[0]);
	this->_request_start_line.uri = this->_extract_uri(decoded_uri);
	this->_request_start_line.target = this->_extract_target(decoded_uri);
	this->_request_start_line.query = this->_extract_query(decoded_uri);
	this->_request_start_line.protocol = request_line_tokens[2]; // lower priority might want to validate
}

void Request::_validate_header_field(const std::pair<std::string, std::string> &header_pair)
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

void Request::_handle_duplicate_headers(const std::string &key, std::string &original_value, const std::string &duplicate_value)
{
	if (key == "Content-Length")
	{
		if (original_value == duplicate_value)
			return;
		else
			throw HTTPException(400, "Multiple content-length headers");
	}
}

void Request::_parse_request_headers()
{
	std::string line;
	std::size_t separator_index;
	std::size_t whitespace_index;
	std::pair<std::string, std::string> header_field_pair;
	std::pair<std::map<std::string, std::string>::iterator,bool> insert_return;

	while (getline_CRLF(this->_buffer_stream, line) && !line.empty())
	{
		separator_index = line.find(':');
		whitespace_index = line.find_first_of(" \n\t\f\r\v");
		if (whitespace_index < separator_index)
			throw HTTPException(400, "Whitespace found before : during header parsing");
		header_field_pair = extract_key_value_pair(line, ':');
		this->_validate_header_field(header_field_pair);
		if (header_field_pair.first == "Host")
		{
			header_field_pair.second = prune_http_protocol(header_field_pair.second);
			header_field_pair.second = url_decode(header_field_pair.second);
		}
		insert_return = this->_headers.insert(header_field_pair);
		if (insert_return.second == false)
			this->_handle_duplicate_headers((*insert_return.first).first, (*insert_return.first).second, header_field_pair.second);
    }
	getline_CRLF(this->_buffer_stream, line);
}

void Request::_parse_chunked_request_body()
{
    std::string line;
    std::string chunk_size_str;
    std::size_t chunk_size = 0;

	while (getline_CRLF(this->_buffer_stream, line) && !line.empty())
	{
        std::istringstream chunkLineStream(line);
        chunkLineStream >> std::hex >> chunk_size_str;

        if (chunkLineStream.fail()) {
            // Invalid chunk size format
            std::cerr << "Invalid chunk size format: " << line << std::endl;
            return;
        }

        chunk_size = hex_str_to_ulong(chunk_size_str);

        if (chunk_size == 0) {
            // End of chunks
            break;
        }

        std::vector<char> chunk_data(chunk_size);
        this->_buffer_stream.read(chunk_data.data(), chunk_size);

		if (chunk_size != chunk_data.size())
		{
			throw HTTPException(400, "Chunk size and chunk data mismatch");
		}
	
        this->_body.write(chunk_data.data(), chunk_size);
		getline_CRLF(this->_buffer_stream, line);
    }
}

void Request::_parse_encoded_request_body()
{
	this->_parse_chunked_request_body();
	// this->_parse_request_headers();
}

void Request::_parse_request()
{
	this->_parse_request_line();
	this->_parse_request_headers();
	if (this->_headers.find("Host") == this->_headers.end())
		throw HTTPException(400 , "Host field not found.");
	this->_headers.insert(std::make_pair("Content-Type", "text/plain; charset=US-ASCII"));
}

#pragma endregion ClassUtility

#pragma region Getters

std::string Request::get_method() const
{
	return this->_request_start_line.method;
}

std::string Request::get_uri() const
{
	return this->_request_start_line.uri;
}

std::string Request::get_target() const
{
	return this->_request_start_line.target;
}

std::string Request::get_protocol() const
{
	return this->_request_start_line.protocol;
}

std::size_t Request::get_body_size() const
{
	std::size_t body_size = 0;
	std::size_t header_break_index = this->_buffer_stream.str().find(CRLF CRLF);

	if (header_break_index == std::string::npos)
		return body_size;
	body_size = this->_buffer_stream.str().length() - body_size + 5;
	return body_size;
}

std::string Request::get_header(const std::string &key) const
{
	std::map<std::string, std::string>::mapped_type	value;
	std::map<std::string, std::string>::const_iterator	it;

	it = this->_headers.find(key);
	value = (it != this->_headers.end()) ? it->second : "" ;
	return value;
}

std::map<std::string, std::string> Request::get_headers() const
{
	return this->_headers;
}

std::stringstream const &Request::get_body_stream() const
{
	return this->_body;
}

std::stringstream &Request::get_body_stream()
{
	return this->_body;
}

std::string Request::get_body_string() const
{
	return this->_body.str();
}

std::string Request::get_query_param(const std::string key) const
{
	std::string value;
	std::map<std::string, std::string>::const_iterator	it;
	
	it = this->_request_start_line.query.find(key);
	value = (it != this->_request_start_line.query.end()) ? it->second : "";
	return value;
}

Request::RequestReadState Request::get_request_read_state() const
{
	return this->_read_state;
}

#pragma endregion Getters

#pragma region Setters

void Request::set_request_read_state(Request::RequestReadState state)
{
	this->_read_state = state;
}

#pragma endregion Setters

#pragma region PublicMemberMethods

void Request::append_to_request(const char *string)
{
	this->_buffer_stream << string;
}

void Request::process_request()
{
	std::size_t header_break_index = this->_buffer_stream.str().find(CRLF CRLF);

	if (header_break_index == std::string::npos)
		return;
	try
	{
		if (this->_headers.size() == 0)
		{
			this->_parse_request();
		}

		std::map<std::string, std::string>::iterator transfer_encoding;
		std::map<std::string, std::string>::iterator content_length;

		transfer_encoding = this->_headers.find("Transfer-Encoding");
		content_length = this->_headers.find("Content-Length");
		if (transfer_encoding == this->_headers.end()
		&& content_length == this->_headers.end())
		{
			this->_read_state = FINISHED;
			return;
		}
		if (transfer_encoding != this->_headers.end())
		{
			std::vector<std::string> temp = tokenise_str(transfer_encoding->second);
			if (temp[temp.size() - 1] != "chunked")
			{
				throw HTTPException(400, "Chunked was not the last encoding type.");
			}
			else if (this->_buffer_stream.str().rfind("0" CRLF CRLF) != std::string::npos)
			{
				this->_parse_encoded_request_body();
				this->_read_state = FINISHED;
			}
		}
		else if ((content_length != this->_headers.end())
				&& (std::strtoul(content_length->second.c_str(), NULL, 10) == (this->_buffer_stream.str().length() - header_break_index + 4)))
		{
			this->_body << this->_buffer_stream.str().substr(header_break_index + 4);
			this->_read_state = FINISHED;
		}
	}
	catch (const HTTPException& e)
	{
		this->_read_state = FINISHED;
		throw e;
	}	
}

#pragma endregion PublicMemberMethods

}
