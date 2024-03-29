#include "RequestLine.hpp"

namespace ft
{
	
RequestLine::RequestLine() : _start_line(), _method(), _target(), _protocol(){}

RequestLine::RequestLine(const std::string &request_line) : _start_line(request_line), _method(), _target(), _protocol()
{
	this->_construct();
}

RequestLine::~RequestLine(){}

RequestLine::RequestLine(const RequestLine& ref) : _start_line(ref._start_line), _method(ref._method), _target(ref._target), _protocol(ref._protocol)
{ 
	*this = ref;
}

RequestLine &RequestLine::operator=(const RequestLine& ref)
{
	if (this != &ref)
	{
		this->_start_line = ref._start_line;
		this->_method = ref._method;
		this->_target = ref._target;
		this->_protocol = ref._protocol;
	}
	return *this;
}

std::string RequestLine::get_start_line() const
{
	return this->_start_line;
}

std::string RequestLine::get_method() const
{
	return this->_method;
}

std::string RequestLine::get_target() const
{
	return this->_target;
}

std::string RequestLine::get_protocol() const
{
	return this->_protocol;
}

std::string RequestLine::get_target_file() const
{
	std::string	decoded_target;
	std::size_t	dot_pos;
	std::size_t	leading_backslash_pos;
	std::size_t	trailing_backslash_pos;
	std::string	target_file_name;

	decoded_target = url_decode(this->_target);
	decoded_target = decoded_target.substr(0, decoded_target.find_first_of('?'));
	dot_pos = decoded_target.find_first_of('.');
	if (dot_pos == std::string::npos)
	{
		return target_file_name;
	}
	leading_backslash_pos = decoded_target.find_last_of('/', dot_pos);
	trailing_backslash_pos = decoded_target.find_first_of('/', dot_pos);
	target_file_name = decoded_target.substr(leading_backslash_pos + 1, trailing_backslash_pos - leading_backslash_pos - 1);
	return target_file_name;
}

std::map<std::string, std::string> RequestLine::get_query_map() const
{
	std::string							decoded_target;
	std::map<std::string, std::string>	result;
	std::size_t							query_begin_index;
	std::size_t							next_index;
	std::pair<std::string, std::string>	key_value_pair;

	decoded_target = url_decode(this->_target);
	query_begin_index = decoded_target.find('?');
	while (query_begin_index != std::string::npos)
	{
		next_index = decoded_target.find('&', query_begin_index + 1);
		key_value_pair = extract_key_value_pair(decoded_target.substr(query_begin_index + 1, next_index), '=');
		query_begin_index = next_index;
		if (key_value_pair.first == "")
		{
			continue;
		}
		result[key_value_pair.first] = key_value_pair.second;
	}
	return result;
}

std::string RequestLine::get_query_string() const
{
	std::string	decoded_target;

	decoded_target = url_decode(this->_target);
	return decoded_target.substr(decoded_target.find_last_of('?') + 1);
}

float RequestLine::get_protocol_version() const
{
	float				result;
	std::stringstream	ss;

	ss << this->_protocol.substr(this->_protocol.find_last_of('/') + 1);
	ss >> result;

	return result;
}

std::string RequestLine::get_decoded_target() const
{
	return url_decode(this->_target);
}

void RequestLine::construct(const std::string &request_line)
{
	this->reset();
	this->_start_line = request_line;
	this->_construct();
}

void RequestLine::reset()
{
	this->_start_line = "";
	this->_method = "";
	this->_target = "";
	this->_protocol = "";
}

void RequestLine::_construct()
{
	std::vector<std::string>	request_line_tokens;

	request_line_tokens = tokenise_str(this->_start_line);
	if (request_line_tokens.size() != 3)
	{
		throw HTTPException(400, "Invalid request start line");
	}

	request_line_tokens = tokenise_str(this->_start_line);
	this->_method = trim_chars(request_line_tokens[0], WHITESPACE_CHARACTERS);
	this->_target = trim_chars(request_line_tokens[1], WHITESPACE_CHARACTERS);
	this->_protocol = trim_chars(request_line_tokens[2], WHITESPACE_CHARACTERS);
	this->_validate_method();
	this->_validate_target();
}

void RequestLine::_validate_target()
{
	std::string decoded_target;
	int			extension_count;

	decoded_target = url_decode(this->_target);
	decoded_target = decoded_target.substr(0, decoded_target.find('?'));
	extension_count = std::count(decoded_target.begin(), decoded_target.end(), '.');
	if (extension_count > 1)
	{
		throw HTTPException(404, "Invalid url");
	}
}

void RequestLine::_validate_method()
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
		if (this->_method.compare(valid_methods[i]) == 0)
		{
			return;
		}
	}
	throw HTTPException(400, "Invalid method");
}

}
