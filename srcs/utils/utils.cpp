#include <cstring>
#include <cerrno>
#include <sstream>
#include <utility>
#include <string>
#include <vector>
#include <string>
#include <algorithm>

std::string ret_str_perror(const std::string &msg)
{
	std::stringstream ss;
	std::string str;

	ss << msg << ": " << std::strerror(errno) << std::endl;
	str = ss.str();
	return str;
}

std::string trim_str(const std::string &str, const char *chars)
{
	std::size_t start_index;
	std::size_t end_index;

	start_index = str.find_first_not_of(chars);
	start_index = (start_index == std::string::npos ? 0 : start_index);
	end_index = str.find_last_not_of(chars);
	end_index = (end_index == std::string::npos ? str.size() : end_index);
	
	return str.substr(start_index, end_index - start_index + 1);
}

std::vector<std::string> tokenise_str(const std::string &str, char c = ' ')
{
	std::string temp;
	std::vector<std::string> ret_vector;
	std::stringstream ss(str);

	while (std::getline(ss, temp, c))
	{
		if (!temp.empty())
		{
			ret_vector.push_back(trim_str(temp, " \n\t"));
		}
	}
	return ret_vector;
}

std::string str_char_limit_span(const std::string &str, char open, char close){
	std::size_t start_index;
	std::size_t delimiter_index;
	std::size_t pair;
	std::string ret;
	std::string span_limiters;

	start_index = str.find_first_of(open);
	if (start_index == std::string::npos)
		return std::string();
	pair = 1;
	start_index += 1;
	delimiter_index = start_index;
	span_limiters += open;
	span_limiters += close;
	while ((pair > 0)
			&& (delimiter_index = str.find_first_of(span_limiters, delimiter_index)) != std::string::npos)
	{
		if (str[delimiter_index] == open){
			pair += 1;
		}
		else {
			pair -= 1;
		}
		++delimiter_index;
	}
	if (pair > 0)
    	return std::string(); 
	ret += str.substr(start_index, delimiter_index - start_index - 1);
	return ret;
}

std::pair<std::string, std::string> extract_key_value_pair(const std::string &str, char delimiter_key)
{
	std::size_t delimiter_index;

	delimiter_index = str.find(delimiter_key);
	if (delimiter_index == std::string::npos)
	{
		throw std::invalid_argument("No delimiter found");
	}
	return std::make_pair(trim_str(str.substr(0, delimiter_index), " "), trim_str(str.substr(delimiter_index + 1), " "));
}

unsigned long hex_str_to_ulong(const std::string &hex_str)
{
	std::istringstream iss(hex_str);
    unsigned long value = 0;

    if (!(iss >> std::hex >>value))
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

    while (input.get(currentChar)) {
        if (prevChar == '\r' && currentChar == '\n')
            return true;  // CRLF found, line complete
		if (prevChar)
            line += prevChar;
		prevChar = currentChar;
    }

    // Check if the last character is part of a line
    if (prevChar != '\0') {
        line += prevChar;
    }

    return false;  // Line incomplete if empty
}

std::string url_decode(const std::string &encoded_url)
{
    std::string decodedUrl;
    std::istringstream iss(encoded_url);

    char ch;
    int hexValue;
    while (iss.get(ch)) {
        if (ch == '%') {
            // Read the next two characters as hexadecimal value
            std::string hexDigits;

            if (iss.get(ch) && std::isxdigit(ch)) {
                hexDigits += ch;
            }
            if (iss.get(ch) && std::isxdigit(ch)) {
                hexDigits += ch;
            }

            if (hexDigits.length() == 2) {
                std::istringstream hexStream(hexDigits);
                hexStream >> std::hex >> hexValue;
                decodedUrl += static_cast<char>(hexValue);
            } else {
                // Invalid encoding, return original character
                decodedUrl += '%';
                decodedUrl += hexDigits;
            }
        } 
		else if (ch == '+')
			decodedUrl += ' ';
		else
            decodedUrl += ch;
    }
    return decodedUrl;
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

std::string size_t_to_string(std::size_t val)
{
	std::ostringstream oss;

	oss << val;

	return oss.str();
}

std::string extract_file_extension(const std::string &filename)
{
	std::size_t extension_begin_index;

	if ((extension_begin_index = filename.find_last_of('.')) == std::string::npos)
	{
		throw std::runtime_error("Missing delimiter for beginning of extension");
	}
	return filename.substr(extension_begin_index + 1);
}

void str_to_uppercase(std::string &str)
{
	std::transform(str.begin(), str.end(), str.begin(), toupper);
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
