#include "Response.hpp"

namespace ft
{

const std::map<int, std::string> Response::reason_phrase_map = Response::_fill_reason_phrase_map();

const std::map<int, std::string> Response::_fill_reason_phrase_map()
{
	std::map<int, std::string> container;

	container[100] = "Continue";
	container[101] = "Switching Protocols";
	container[102] = "Processing";
	container[200] = "OK";
	container[201] = "Created";
	container[202] = "Accepted";
	container[203] = "Non-Authoritative Information";
	container[204] = "No Content";
	container[205] = "Reset Content";
	container[206] = "Partial Content";
	container[207] = "Multi-Status";
	container[208] = "Already Reported";
	container[226] = "IM Used";
	container[300] = "Multiple Choices";
	container[301] = "Moved Permanently";
	container[302] = "Found";
	container[303] = "See Other";
	container[304] = "Not Modified";
	container[305] = "Use Proxy";
	container[306] = "Switch Proxy";
	container[307] = "Temporary Redirect";
	container[308] = "Permanent Redirect";
	container[400] = "Bad Request";
	container[401] = "Unauthorized";
	container[402] = "Payment Required";
	container[403] = "Forbidden";
	container[404] = "Not Found";
	container[405] = "Method Not Allowed";
	container[406] = "Not Acceptable";
	container[407] = "Proxy Authentication Required";
	container[408] = "Request Timeout";
	container[409] = "Conflict";
	container[410] = "Gone";
	container[411] = "Length Required";
	container[412] = "Precondition Failed";
	container[413] = "Payload Too Large";
	container[414] = "URI Too Long";
	container[415] = "Unsupported Media Type";
	container[416] = "Range Not Satisfiable";
	container[417] = "Expectation Failed";
	container[418] = "I'm a teapot";
	container[421] = "Misdirected Request";
	container[422] = "Unprocessable Entity";
	container[423] = "Locked";
	container[424] = "Failed Dependency";
	container[426] = "Upgrade Required";
	container[428] = "Precondition Required";
	container[429] = "Too Many Requests";
	container[431] = "Request Header Fields Too Large";
	container[451] = "Unavailable For Legal Reasons";
	container[500] = "Internal Server Error";
	container[501] = "Not Implemented";
	container[502] = "Bad Gateway";
	container[503] = "Service Unavailable";
	container[504] = "Gateway Timeout";
	container[505] = "HTTP Version Not Supported";
	container[506] = "Variant Also Negotiates";
	container[507] = "Insufficient Storage";
	container[508] = "Loop Detected";
	container[510] = "Not Extended";
	container[511] = "Network Authentication Required";
	container[520] = "Unknown Error";
	container[522] = "Origin Connection Time-out";
	container[598] = "Network read timeout error";
	container[599] = "Network connect timeout error";
	return container;
}

Response::Response() : _http_protocol(), _status_code(), _headers(), _message_format(NULL), _body_stream(NULL)
{}

Response::Response(float protocol_version) : _http_protocol(protocol_version), _status_code(), _headers(), _message_format(NULL), _body_stream(NULL)
{}

Response::~Response(){}


int	Response::get_status_code() const
{
	return this->_status_code;
}

std::istream const *Response::get_body_stream() const
{
	return this->_body_stream.get();
}

std::istream *Response::get_body_stream() 
{
	return this->_body_stream.get();
}

std::istream *Response::get_message_format() const
{
	return this->_message_format.get();
}

void Response::set_status_code(const int &code)
{
	this->_status_code = code;
}

void Response::set_body_stream(std::istream *body_stream)
{
	this->_body_stream = std::auto_ptr<std::istream>(body_stream);
}

void Response::set_body_stream(std::auto_ptr<std::istream> body_stream)
{
	this->_body_stream = body_stream;
}

void Response::set_header(const std::string &key, const std::string &value)
{
	std::cerr << "inside setting header" << std::endl;
	std::cerr << "key is: " << key << " value is: " << value << std::endl;;
	this->_headers[key] = value;
	std::cerr << "after setting header" << std::endl;
}

void Response::append_headers(const std::map<std::string, std::string> &header_map)
{
	for (std::map<std::string, std::string>::const_iterator it = header_map.begin(); it != header_map.end(); it++)
	{
		this->_headers.insert(*it);
	}
}

void Response::remove_header(const std::string &key)
{
	this->_headers.erase(key);
}

void Response::construct_response_message_format() // TODO: refactor
{
	std::cerr << "in construct_response_message_format" << std::endl;

	if (this->_message_format.get() == NULL)
	{
		this->_message_format.reset(new std::stringstream);
	}

	*this->_message_format << "HTTP/" << this->_http_protocol << " " << this->_status_code << " " << (this->reason_phrase_map.find(this->_status_code)->second) << "\r\n";
	for (std::map<std::string, std::string>::const_iterator cit = this->_headers.begin(); cit != this->_headers.end(); cit++)
	{
		*this->_message_format << cit->first << ": " << cit->second << "\r\n";
	}
	*this->_message_format << "\r\n";
}

std::string Response::read_response(std::size_t buffer_size)
{
	std::auto_ptr<char> buffer(new char[buffer_size]);
	std::size_t			remaining_char_count = buffer_size;
	std::size_t			buffer_offset = 0;

	std::memset(buffer.get(), 0, buffer_size);
	if (*(this->_message_format))
	{
		std::cerr << "reading unfinished message format" << std::endl;
		if (this->_message_format->str().size() < buffer_size)
		{
			std::cerr << "buffer greater than message format size" << std::endl;
			remaining_char_count -= this->_message_format->str().size();
			buffer_offset += this->_message_format->str().size();
		}
		this->_message_format->read(buffer.get(), remaining_char_count);
	}
	if (!(*(this->_message_format)) && this->_body_stream.get() != NULL)
	{
		std::cerr << "before read pos: " << this->_body_stream->tellg() << std::endl;
		this->_body_stream->read(buffer.get() + buffer_offset, remaining_char_count);
		std::cerr << "after read pos: " << this->_body_stream->tellg() << std::endl;
	}
	std::cerr << "string read from read response: " << std::string(buffer.get()) << std::endl;
	return std::string(buffer.get());
}

void Response::unread_response(std::size_t unread_size)
{

	std::size_t	current_message_format_pos;

	current_message_format_pos = static_cast<std::size_t>(this->_message_format->tellg());
	if (!(*this->_message_format.get()) && this->_body_stream.get() != NULL)
	{
		std::size_t	current_body_stream_pos;

		current_body_stream_pos = static_cast<std::size_t>(this->_body_stream->tellg());
		if (!(*this->_body_stream.get()))
		{
			this->_body_stream->clear();
		}

		if (current_body_stream_pos >= unread_size)
		{
			this->_body_stream->seekg(current_body_stream_pos - unread_size);
			return;
		}
		else
		{
			unread_size -= current_body_stream_pos;
			this->_body_stream->seekg(0);
			this->_message_format->clear();
		}
	}
	if (current_message_format_pos >= unread_size)
	{
		this->_message_format->seekg(current_message_format_pos - unread_size);
	}
	else 
	{
		this->_message_format->seekg(0);
	}
}

bool Response::has_been_completely_read() const
{
	bool bodystream_read_status = false;

	if (this->_body_stream.get() == NULL)
	{
		std::cerr << "setting body stream read status" << std::endl;
		bodystream_read_status = true;
	}
	else
	{
		std::cerr << "before segfault" << std::endl;
		bodystream_read_status = !(*this->_body_stream);
	}
	return !(*this->_message_format) && bodystream_read_status;
}

}
