#include <cstring>
#include <cerrno>
#include <sstream>
#include <utility>
#include <string>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

#include "utils.hpp"

namespace ft
{

std::string ltrim_chars(const std::string& str, const std::string &characters)
{
	std::size_t begin_pos;
	
	begin_pos = str.find_first_not_of(characters) % std::string::npos;
    return str.substr(begin_pos);
}

std::string rtrim_chars(const std::string& str, const std::string &characters)
{
	std::size_t last_pos;
	
	last_pos = str.find_last_not_of(characters);
	if (last_pos != std::string::npos)
	{
	    last_pos += 1;
	}
    return str.substr(0, last_pos);
}

std::string trim_chars(const std::string &str, const std::string &chars)
{
	return ltrim_chars(rtrim_chars(str, chars), chars);
}

std::string ret_str_perror(const std::string &msg)
{
	std::stringstream ss;
	std::string str;

	ss << msg << ": " << std::strerror(errno) << std::endl;
	str = ss.str();
	return str;
}

std::vector<std::string> tokenise_str(const std::string &str, char c)
{
	std::string temp;
	std::vector<std::string> ret_vector;
	std::stringstream ss(str);

	while (std::getline(ss, temp, c))
	{
		if (!temp.empty())
		{
			ret_vector.push_back(trim_chars(temp, WHITESPACE_CHARACTERS));
		}
	}
	return ret_vector;
}

std::string str_limit_span(const std::string &str, const char open, const char close){
	std::size_t start_index;
	std::size_t delimiter_index;
	std::size_t pair = 0;
	std::string ret = "";

	if (open == 0 || close == 0 || open == close)
    {
        return ret;
    }
	start_index = str.find_first_of(open);
	if (start_index == std::string::npos)
		return ret;
	pair = 1;
	start_index += 1;
	delimiter_index = start_index;
	while ((pair > 0)
			&& (delimiter_index = str.find_first_of(std::string(1, open) + std::string(1, close), delimiter_index)) != std::string::npos)
	{
		if (str[delimiter_index] == open){
			++pair;
		}
		else if (str[delimiter_index] == close) 
		{
			--pair;
		}
		if (pair == 0)
		{
			break;
		}
		++delimiter_index;
	}
	if (pair > 0)
    	return ret; 
	ret = str.substr(start_index, delimiter_index - start_index);
	return ret;
}

std::string extract_between_boundaries(const std::string &text, const std::string &start_boundary, const std::string &end_boundary)
{
    std::size_t start_index = text.find(start_boundary);
    if (start_index == std::string::npos)
	{
        return "";
    }
	 
	start_index += start_boundary.length();

    std::size_t end_index = text.find(end_boundary, start_index);
    if (end_index == std::string::npos)
	{
        return "";
    }

    return text.substr(start_index, end_index - start_index);
}

std::pair<std::string, std::string> extract_key_value_pair(const std::string &str, const char delimiter_key)
{
	std::size_t							delimiter_index;
	std::pair<std::string, std::string>	result;

	delimiter_index = str.find(delimiter_key);
	if (delimiter_index == std::string::npos)
	{
		return result;
	}
	result.first = trim_chars(str.substr(0, delimiter_index), WHITESPACE_CHARACTERS);
	result.second = trim_chars(str.substr(delimiter_index + 1), WHITESPACE_CHARACTERS);
	return result;
}

unsigned long hex_str_to_ulong(const std::string &hex_str)
{
	std::istringstream iss(hex_str);
    unsigned long value = 0;

    if (!(iss >> std::hex >> value))
    {
		throw std::logic_error("Unable to convert from hex to ulong");
    }

    return value;
}

bool getline_CRLF(std::istream& input, std::string& line)
{
    line.clear();

    char prevChar = '\0';
    char currentChar;

    while (input.get(currentChar))
	{
        if (prevChar == '\r' && currentChar == '\n')
		{
            return true;  // CRLF found, line complete
		}
		if (prevChar)
		{
            line += prevChar;
		}
		prevChar = currentChar;
    }

    // Check if the last character is part of a line
    if (prevChar != '\0')
	{
        line += prevChar;
    }

    return false;  // Line incomplete if empty
}

std::string url_decode(const std::string &encoded_url)
{
    std::string			decoded_url;
    std::istringstream	iss(encoded_url);
    char				ch;
    int					hex_value;

    while (iss.get(ch)) {
        if (ch == '%') {
            std::string hex_digits;
			
			for (int i = 0; i < 2; i++)
			{
				iss.get(ch);
                if (i == 1 && !std::isxdigit(ch))
                {
                    iss.unget();
                    break;
                }
                hex_digits += ch;
                if (!std::isxdigit(ch))
                    break;
            }

            if (hex_digits.length() == 2)
			{
                std::istringstream hex_stream(hex_digits);
                hex_stream >> std::hex >> hex_value;
                decoded_url += static_cast<char>(hex_value);
            } 
			else
			{
                decoded_url += '%';
                decoded_url += hex_digits;
            }
        } 
		else if (ch == '+')
		{
			decoded_url += ' ';
		}
		else
		{
            decoded_url += ch;
		}
    }
    return decoded_url;
}

std::string string_vector_to_string(const std::vector<std::string> &vect, char delimiter = ' ')
{
	std::stringstream ss;
  
    for (std::vector<std::string>::const_iterator cit = vect.begin(); cit != vect.end() ;cit++) {
        ss << *cit << delimiter;
    }
    return ss.str();
}

std::string prune_http_protocol(const std::string &domain_str)
{
	if (domain_str.find("https:\\\\") == std::string::npos)
		return domain_str;
	else if (domain_str.find("http:\\\\") == std::string::npos)
		return domain_str;
	return domain_str.substr(domain_str.find_last_of('\\'), 7);
}

std::size_t	calc_input_stream_size(std::istream &stream)
{
	std::streampos current_pos = stream.tellg();
	std::size_t	size;

	stream.seekg(0, stream.end);
	size = stream.tellg();
	stream.seekg(current_pos, stream.beg);

	return size;
}

std::string str_to_uppercase(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), toupper);
	return str;
}

void str_replace_char(std::string &str, char old_char, char new_char)
{
	for (std::string::iterator it = str.begin(); it != str.end(); it++)
	{
		if (*it == old_char)
		{
			*it = new_char;
		}
	}
}

std::string extract_file_extension(const std::string &filename)
{
	std::size_t dot_index = filename.find_last_of('.');
	if (dot_index == std::string::npos)
	{
		return "";
	}
	return filename.substr(dot_index + 1);
}

void	close_fd_helper(int fd)
{
	close(fd);
}

std::map<std::string, std::string> parse_key_value_string(const std::string &kv_string, const char kv_seperator , const char kv_delim)
{
	std::istringstream 					iss(kv_string);
	std::string							pair_string;
	std::map<std::string, std::string>	result;
	std::pair<std::string, std::string> extracted_pair;

	if (kv_string.find(kv_seperator) == std::string::npos)
	{
		return result;
	}
	while (std::getline(iss, pair_string, kv_delim)) {
        std::istringstream	pair_stream(pair_string);
        std::string			key, value;
        
        if (std::getline(std::getline(pair_stream, key, kv_seperator), value))
		{
            result[key] = value;
        }
    }
	return result;
}

bool is_system_compatible_filename(const std::string &filename)
{
	if (filename.find_first_of("\\/:*?\"<>|") != std::string::npos) // windows limitation
	{
        return false;
    }
    if (filename.length() > PATH_MAX - 1) {
        return false;
    }
    return true;
}
	
}
