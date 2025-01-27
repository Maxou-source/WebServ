
#include "MessageContext.hpp"
#include "ServerData.hpp"

bool	MessageContext::isInformational(int code)	{ return (code >= 100 && code < 200); }
bool	MessageContext::isSuccessful(int code)		{ return (code >= 200 && code < 300); }
bool	MessageContext::isRedirection(int code)		{ return (code >= 300 && code < 400); }
bool	MessageContext::isClientError(int code)		{ return (code >= 400 && code < 500); }
bool	MessageContext::isServerDataError(int code)		{ return (code >= 500 && code < 600); }
bool	MessageContext::isError(int code)			{ return (code >= 400); }

std::string MessageContext::reasonPhrase(int code)
{
	switch (code)
	{

	case 100: return "Continue";
	case 101: return "Switching Protocols";
	case 102: return "Processing";
	case 103: return "Early Hints";

	case 200: return "OK";
	case 201: return "Created";
	case 202: return "Accepted";
	case 203: return "Non-Authoritative Information";
	case 204: return "No Content";
	case 205: return "Reset Content";
	case 206: return "Partial Content";
	case 207: return "Multi-Status";
	case 208: return "Already Reported";
	case 226: return "IM Used";

	case 300: return "Multiple Choices";
	case 301: return "Moved Permanently";
	case 302: return "Found";
	case 303: return "See Other";
	case 304: return "Not Modified";
	case 305: return "Use Proxy";
	case 307: return "Temporary Redirect";
	case 308: return "Permanent Redirect";

	case 400: return "Bad Request";
	case 401: return "Unauthorized";
	case 402: return "Payment Required";
	case 403: return "Forbidden";
	case 404: return "Not Found";
	case 405: return "Method Not Allowed";
	case 406: return "Not Acceptable";
	case 407: return "Proxy Authentication Required";
	case 408: return "Request Timeout";
	case 409: return "Conflict";
	case 410: return "Gone";
	case 411: return "Length Required";
	case 412: return "Precondition Failed";
	case 413: return "Content Too Large";
	case 414: return "URI Too Long";
	case 415: return "Unsupported Media Type";
	case 416: return "Range Not Satisfiable";
	case 417: return "Expectation Failed";
	case 418: return "I'm a teapot";
	case 421: return "Misdirected Request";
	case 422: return "Unprocessable Content";
	case 423: return "Locked";
	case 424: return "Failed Dependency";
	case 425: return "Too Early";
	case 426: return "Upgrade Required";
	case 428: return "Precondition Required";
	case 429: return "Too Many Requests";
	case 431: return "Request Header Fields Too Large";
	case 451: return "Unavailable For Legal Reasons";

	case 500: return "Internal ServerData Error";
	case 501: return "Not Implemented";
	case 502: return "Bad Gateway";
	case 503: return "Service Unavailable";
	case 504: return "Gateway Timeout";
	case 505: return "HTTP Version Not Supported";
	case 506: return "Variant Also Negotiates";
	case 507: return "Insufficient Storage";
	case 508: return "Loop Detected";
	case 510: return "Not Extended";
	case 511: return "Network Authentication Required";

	default: return std::string();
	}
}

std::string	MessageContext::intToString(ssize_t to_string) {
	std::ostringstream	convert;

	convert << to_string;
	return (convert.str());
}


std::string	MessageContext::responseHeader(int code, bool notFinished) {
	std::ostringstream	convert;
	std::string			header;

	header = PROTOCOL_VERSION;
	header += " ";
	convert << code;
	header += convert.str();
	header += " ";
	header += reasonPhrase(code);
	header += (notFinished == false) ? "\r\n\r\n" : "\r\n";
	return (header);
}

std::string MessageContext::errorPage(int _code)
{
	std::string _body = "<h1>" + intToString(_code) + " " + reasonPhrase(_code) + "</h1>";
	return (responseHeader(_code, true) + "Content-Length: " + intToString(_body.size()) + "\r\nContent-Type: text/html" +  "\r\n\r\n" + _body);
}

std::string MessageContext::redirection(int _code, ServerData *serv)
{
	return (responseHeader(_code, true) + 
			"Location: " + serv->getRedirect() + "\r\n\r\n");
}

std::string MessageContext::directoryListing(std::string & directoryPath)
{
	std::string result(responseHeader(CODE_OK, true));
	std::string _body("<head><br><h1>MaxoU</h1><br></head><body> <ul> \n");
	DIR* dir = opendir(directoryPath.c_str());
	if (dir != NULL) {
		struct dirent* entry;
		while ((entry = readdir(dir)) != NULL) {
			std::string fileName = entry->d_name;
			if (fileName != "." && fileName != "..") {
				_body += ("<li href=\"" + directoryPath + fileName +"\">" + fileName + "</li>");
			}
		}
		closedir(dir);
	}
	_body += "\n</ul> </body>";
	return result + "Content-Length: " + intToString(_body.size()) + "\r\nContent-Type: text/html\r\n\r\n" + _body ;
}

MessageContext &MessageContext::getInstance() {
	static MessageContext instance;
	return instance;
}

std::string	&MessageContext::getErrorContent() {
	return _errorContent;
}
void		MessageContext::setErrorContent(std::string &ec) {
	_errorContent = ec;
}

bool MessageContext::isADirectory(std::string& path)
{
	DIR* dir = opendir(path.c_str());
	if (dir != NULL) {
		closedir(dir);
		return true;
	}
	return false;
}
