#include "Request.hpp"

namespace ft
{

#pragma region Constructor

Request::Request() : _ss(), _request_start_line(), _headers(), _body(), _read_state(Request::READING){}

Request::Request(const std::string &request_string) : _ss(), _request_start_line(), _headers(), _body(), _read_state(Request::READING){}

Request::Request(const Request &ref) : _ss(ref._ss), _request_start_line(ref._request_start_line), _headers(ref._headers), _body(ref._body)
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
		this->_ss = ref._ss;
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
	throw std::runtime_error("Invalid method");
}

std::string Request::_extract_uri(const std::string &uri_string)
{
	return ft::url_decode(uri_string.substr(0, uri_string.find('?')));
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
			break;
		}
		query_begin_index = next_index;
	}
	return return_map;
}

void Request::_parse_request_line()
{
	std::vector<std::string> request_line_tokens;
	std::string line;

	ft::getline_CRLF(this->_ss, line);
	request_line_tokens = tokenise_str(line);
	if (request_line_tokens.size() != 3)
	{
		std::runtime_error("Invalid request start line");
	}

	this->_request_start_line.method = this->_validate_method(request_line_tokens[0]);
	this->_request_start_line.uri = this->_extract_uri(request_line_tokens[1]);
	this->_request_start_line.query = this->_extract_query(request_line_tokens[1]);
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
			throw std::runtime_error("Multiple content-length headers");
	}
}

void Request::_parse_request_headers()
{
	std::string line;
	std::size_t separator_index;
	std::size_t whitespace_index;
	std::pair<std::string, std::string> header_field_pair;
	std::pair<std::map<std::string, std::string>::iterator,bool> insert_return;

	while (getline_CRLF(this->_ss, line) && !line.empty())
	{
		separator_index = line.find(':');
		whitespace_index = line.find_first_of(" \n\t\f\r\v");
		if (whitespace_index < separator_index)
			throw std::runtime_error("Whitespace found before : during header parsing");
		header_field_pair = extract_key_value_pair(line, ':');
		this->_validate_header_field(header_field_pair);
		if (header_field_pair.first == "Host")
			header_field_pair.second = url_decode(header_field_pair.second);
		insert_return = this->_headers.insert(header_field_pair);
		if (insert_return.second == false)
			this->_handle_duplicate_headers((*insert_return.first).first, (*insert_return.first).second, header_field_pair.second);
    }
	getline_CRLF(this->_ss, line);
}

void Request::_parse_chunked_request_body()
{
    std::string line;
	std::ostringstream oss;
    std::string chunk_size_str;
    std::size_t chunk_size = 0;

	while (ft::getline_CRLF(this->_ss, line) && !line.empty())
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
        this->_ss.read(chunk_data.data(), chunk_size);

		if (chunk_size != chunk_data.size())
		{
			throw std::runtime_error("Chunk size and chunk data mismatch");
		}

        // Process the chunk data as needed
        // For example, you can append it to a buffer or write it to a file
        oss.write(chunk_data.data(), chunk_size);
		ft::getline_CRLF(this->_ss, line);
    }
    // Process the complete request
	this->_body = oss.str();
}

void Request::_parse_encoded_request_body()
{
	this->_parse_chunked_request_body();
	this->_parse_request_headers();
}

// void Request::_parse_content_type(const std::string *content_type)
// {
// 	if (content_type == "application/x-www-form-urlencoded")
// 		// do something
// 	else if (content_type == "multipart/form-data")
// 		// do something
// }

void Request::_parse_request()
{
	this->_parse_request_line();
	this->_parse_request_headers();
	if (this->_headers.find("Host") == this->_headers.end())
		throw std::runtime_error("Host field not found.");
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

std::string Request::get_protocol() const
{
	return this->_request_start_line.protocol;
}

std::string Request::get_header(const std::string &key) const
{
	std::map<std::string, std::string>::mapped_type	value;
	std::map<std::string, std::string>::const_iterator	it;

	it = this->_headers.find(key);
	value = (it != this->_headers.end()) ? it->second : "" ;
	return value;
}

std::string Request::get_body() const
{
	return this->_body;
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
	this->_ss << string;
}

void Request::process_request()
{
	std::size_t header_break_index = this->_ss.str().find(CRLF CRLF);

	if (header_break_index == std::string::npos)
		return;
	if (this->_headers.size() == 0)
		this->_parse_request();

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
	else if (transfer_encoding != this->_headers.end()
			&& content_length != this->_headers.end())
		throw std::runtime_error("Both Transfer-Encoding and Content-Length found");
	if ((transfer_encoding != this->_headers.end())
		&& (header_break_index == this->_ss.str().rfind("0" CRLF CRLF)))
	{
		this->_parse_encoded_request_body();
		this->_read_state = FINISHED;
	}
	else if ((content_length != this->_headers.end())
			&& (std::strtoul(content_length->second.c_str(), NULL, 10)== (this->_ss.str().length() - header_break_index + 4)))
	{
		this->_body = this->_ss.str().substr(header_break_index + 4);
		this->_read_state = FINISHED;
	}
	// else if (this->_headers.find("Content-Type") != this->_headers.end())
	// 	this->_parse_content_type(this->_headers["Content-Type"], this->_ss);	
}

#pragma endregion PublicMemberMethods


}
