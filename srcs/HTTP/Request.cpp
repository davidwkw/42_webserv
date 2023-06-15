#include "Request.hpp"

namespace ft
{

#pragma region Constructor

Request::Request() : _request_start_line(), _headers(), _body()
{
}

Request::Request(const std::string &request_string)
{
	this->_parse_request_string(request_string);
}

Request::Request(const Request &src) : _request_start_line(src._request_start_line), _headers(src._headers), _body(src._body)
{
	*this = src;
}

Request::~Request(){}

#pragma endregion Constructor

#pragma region Operator_Overload

Request &Request::operator=(const std::string &request_string)
{
	this->_parse_request_string(request_string);	
}

Request &Request::operator=(const Request &src)
{
	if (this != &src)
	{
		this->_request_start_line = src._request_start_line;
		this->_headers = src._headers;
		this->_body = src._body;
	}
	return *this;
}

#pragma endregion Operator_Overload

#pragma region Class_Utility

void Request::_validate_request_start_line_break(const std::string &headers)
{
	const std::string whitespace_characters = " \n\t\f\r\v";

	std::size_t whitespace_index;
	std::size_t not_whitespace_index;

	whitespace_index = headers.find_first_of(whitespace_characters);
	not_whitespace_index = headers.find_first_not_of(whitespace_characters);
	if (whitespace_index < not_whitespace_index)
		throw std::runtime_error("Whitespace found before CRLF");
}

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

void Request::_parse_request_line(std::istream& iss, std::string& line)
{
	std::vector<std::string> request_line_tokens;
	
	line.clear();
	ft::getline_CRLF(iss, line);
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

void Request::_parse_request_headers(const std::string &header)
{
	std::size_t separator_index;
	std::size_t whitespace_index;
	std::pair<std::string, std::string> header_field_pair;
	std::pair<std::map<std::string, std::string>::iterator,bool> insert_return;
    std::istringstream iss(header);
    std::string line;

	while (ft::getline_CRLF(iss, line) && !line.empty())
	{
		separator_index = line.find(':');
		whitespace_index = line.find_first_of(" \n\t\f\r\v");
		if (whitespace_index < separator_index)
			throw std::runtime_error("Whitespace found before : during header parsing");
		header_field_pair = extract_key_value_pair(line, ':');
		this->_validate_header_field(header_field_pair);
		insert_return = this->_headers.insert(header_field_pair);
		if (insert_return.second == false)
			this->_handle_duplicate_headers((*insert_return.first).first, (*insert_return.first).second, header_field_pair.second);
    }
}

// void Request::_parse_request_headers(const std::string &header)
// {
// 	std::size_t begin_index;
// 	std::size_t end_of_headers_index;
// 	std::size_t end_of_line_index;
// 	std::size_t separator_index;
// 	std::size_t whitespace_index;
// 	std::string header_line;
// 	std::pair<std::string, std::string> header_field_pair;
// 	std::pair<std::map<std::string, std::string>::iterator,bool> insert_return;
	
// 	begin_index = 0;
// 	end_of_line_index = header.find(CRLF);
// 	end_of_headers_index = header.find(CRLF CRLF);
// 	if (end_of_line_index == std::string::npos || end_of_headers_index == std::string::npos)
// 		return;
// 	while (begin_index != end_of_headers_index)
// 	{
// 		header_line = header.substr(begin_index, end_of_line_index);
// 		separator_index = header_line.find(':');
// 		whitespace_index = header_line.find_first_of(" \n\t\f\r\v");
// 		if (whitespace_index < separator_index)
// 			throw std::runtime_error("Whitespace found before : during header parsing");
// 		header_field_pair = extract_key_value_pair(header_line, ':');
// 		this->_validate_header_field(header_field_pair);
// 		insert_return = this->_headers.insert(header_field_pair);
// 		if (insert_return.second == false)
// 			this->_handle_duplicate_headers((*insert_return.first).first, (*insert_return.first).second, header_field_pair.second);
// 		begin_index = end_of_line_index + 2;
// 		end_of_line_index = header.find(CRLF, begin_index);
// 	}
// }

void Request::_parse_chunked_request_body(const std::string &request_body)
{
	std::istringstream iss(request_body);
    std::string line;
	std::ostringstream oss;
    std::string chunk_size_str;
    std::size_t chunk_size = 0;

	while (ft::getline_CRLF(iss, line) && !line.empty())
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
        iss.read(chunk_data.data(), chunk_size);

		if (chunk_size != chunk_data.size())
		{
			throw std::runtime_error("Chunk size and chunk data mismatch");
		}

        // Process the chunk data as needed
        // For example, you can append it to a buffer or write it to a file
        oss.write(chunk_data.data(), chunk_size);
		ft::getline_CRLF(iss, line);
    }

    // Get the remaining part of the request (if any)

    // Process the complete request
	
}

// void Request::_parse_chunked_request_body(const std::string &request_body)
// {
// 	enum ChunkSequence
// 	{
// 		CHUNK_SIZE,
// 		MESSAGE_CHUNK,
// 		CHUNK_SEQUENCE_SIZE
// 	};

// 	long int chunk_size = 0;
// 	std::size_t begin_of_line = 0;
// 	std::size_t end_of_line = request_body.find(CRLF);
// 	std::size_t sequence = 0;

// 	while ()
// 	{
// 		if (sequence == CHUNK_SIZE)
// 		{
// 			chunk_size = std::strtol(request_body.substr(begin_of_line, end_of_line - begin_of_line).c_str() , NULL, 10);
// 			// technically chunk extension can proceed the chunk size hex after ; separated into key value pair by =
// 			if (chunk_size == 0)
// 				break;
// 		}
// 		else if (sequence == MESSAGE_CHUNK)
// 		{
// 			std::string message_chunk = request_body.substr(begin_of_line, end_of_line - begin_of_line);
// 			if (chunk_size != message_chunk.length())
// 				throw std::runtime_error("SOMETHING FUCKED UP. WE'RE MISSING SOME THE REST OF THE MESSAGE");
// 			this->_body += message_chunk;
// 		}
// 		sequence = (sequence + 1) % CHUNK_SEQUENCE_SIZE;
// 		begin_of_line = end_of_line + 2;
// 		end_of_line = request_body.find(CRLF);
// 	}
// }

void Request::_parse_encoded_request_body(const std::string &request_body)
{
	std::size_t trailing_header_begin = request_body.find(CRLF CRLF);
	std::size_t	trailing_header_end = request_body.rfind(CRLF CRLF);

	this->_parse_chunked_request_body(request_body);
	if (trailing_header_begin == trailing_header_end)
		return;
	this->_parse_request_headers(request_body.substr(trailing_header_begin + 4, request_body.rfind(CRLF CRLF) + 4));
}

void Request::_parse_request_string(const std::string &header_section)
{
	std::istringstream iss(header_section);
	std::string line;
	std::map<std::string, std::string>::iterator transfer_encoding;
	std::map<std::string, std::string>::iterator content_length;
	std::size_t begin_index = 0;
	// std::size_t end_of_line_index = header_section.find(CRLF);
	// std::size_t end_of_headers_index = header_section.find(CRLF CRLF);

	if (header_section.find(CRLF) == std::string::npos)
		throw std::runtime_error("Completely missing CRLF");
	if (header_section.find(CRLF CRLF) == std::string::npos)
		throw std::runtime_error("Missing CRLF CRLF break");
	ft::getline_CRLF(iss, line);
	this->_parse_request_line(iss, line);
	ft::getline_CRLF(iss, line);
	if (line.empty())
		return;
	while ()
	// this->_parse_request_line(header_section.substr(begin_index, end_of_line_index));
	// begin_index = end_of_line_index + 2;
	// end_of_line_index = header_section.find(CRLF, begin_index);
	this->_validate_request_start_line_break(header_section.substr(begin_index, end_of_line_index + 2));
	this->_parse_request_headers(header_section.substr(begin_index, end_of_headers_index));
	if (this->_headers.find("Host") == this->_headers.end())
		throw std::runtime_error("Host field not found.");
	transfer_encoding = this->_headers.find("Transfer-Encoding");
	content_length = this->_headers.find("Content-Length");
	if (transfer_encoding == this->_headers.end()
	&& content_length == this->_headers.end())
		return;
	else if (transfer_encoding != this->_headers.end()
			&& content_length != this->_headers.end())
		throw std::runtime_error("Both Transfer-Encoding and Content-Length found");
	// Parsing request body
	if (transfer_encoding != this->_headers.end())
		this->_parse_encoded_request_body(header_section);
	// else if (content_length != this->_headers.end())
	// 	this->_parse_request_body(header_section, content_length);
}

#pragma endregion Class_Utility

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

#pragma endregion Getters

}
