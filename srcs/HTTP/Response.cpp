#include "Response.hpp"

namespace ft
{

#pragma region Non-memberVariable

const std::map<int, std::string> Response::reason_phrase_map = Response::_fill_reason_phrase_map();

const std::map<std::string, std::string> Response::mime_type_map = Response::_fill_mime_type_map();

#pragma endregion Non-memberVariable

#pragma region Non-memberFunction

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

const std::map<std::string, std::string> Response::_fill_mime_type_map()
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


#pragma endregion Non-memberFunction

#pragma region Constructors

Response::Response(const float &protocol, int status_code = 0): _http_protocol(protocol), _status_code(status_code), _body(), _write_state(Response::WRITING){}

Response::Response(const Response &ref)
{
	*this = ref;
}

Response::~Response(){}

#pragma endregion Constructors

#pragma region OperatorOverloads

Response &Response::operator=(const Response &ref)
{
	if (this != &ref)
	{
		this->_http_protocol = ref._http_protocol;
		this->_status_code = ref._status_code;
		this->_body = ref._body;
		this->_headers = ref._headers;
	}
	return (*this);
}

#pragma endregion OperatorOverloads

#pragma region Getters

Response::ResponseWriteState Response::get_write_state() const
{
	return this->_write_state;
}

std::string	Response::get_body() const
{
	return this->_body;
}

int	Response::get_status_code() const
{
	return this->_status_code;
}

std::string Response::get_string() const
{
	return this->format_response().str();
}

#pragma endregion Getters

#pragma region Setters

void Response::set_status_code(const int &code)
{
	this->_status_code = code;
}

void Response::set_body(const std::string &str)
{
	this->_body = str;
}

#pragma endregion Setters

#pragma region PublicMemberFunctions

std::stringstream Response::format_response() const
{
	std::stringstream ss;

	ss << "HTTP/" << this->_http_protocol << " " << this->_status_code << " " << (this->reason_phrase_map.find(this->_status_code)->second) << "\r\n";
	for (std::map<std::string, std::string>::const_iterator cit = this->_headers.begin(); cit != this->_headers.end(); cit++)
	{
		ss << cit->first << ": " << cit->second << "\r\n";
	}
	ss << "\r\n";
	if (this->_headers.find("Transfer-Encoding") != this->_headers.end())
		ss << this->_body;

	return ss;
}

#pragma endregion PublicMemberFunctions

}
