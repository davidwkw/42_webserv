#include <cstring>
#include <cerrno>
#include <sstream>
#include <utility>
#include <string>
#include <vector>
#include <string>

std::string ret_str_perror(const std::string &msg)
{
	std::stringstream ss;
	std::string str;

	ss << msg << ": " << std::strerror(errno) << std::endl;
	str = ss.str();
	return str;
}

std::string trim_str(const std::string &str, char *chars)
{
	std::size_t start_index;
	std::size_t end_index;

	start_index = str.find_first_not_of(chars);
	start_index = (start_index == std::string::npos ? 0 : start_index);
	end_index = str.find_last_not_of(chars);
	end_index = (end_index == std::string::npos ? str.size() : end_index);
	
	return str.substr(start_index, end_index - start_index + 1);
}

std::vector<std::string> tokenise_str(const std::string & str, char c = ' ')
{
	std::string temp;
	std::vector<std::string> ret_vector;
	std::stringstream ss(str);

	while (std::getline(ss, temp, c))
		ret_vector.push_back(trim_str(temp, " \n\t"));
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
