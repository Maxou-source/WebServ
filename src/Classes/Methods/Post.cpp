
#include "Post.hpp"
#include "RequestHandler.hpp"
#include <iomanip>

Post::Post(std::vector<std::string>& vec, std::string *	_body, ServerData* serv)
: Method(vec, _body, serv), _contentType("") {
	_type = HTTP_POST;
}

std::string Post::virtualResponse(void) {
	return (std::string());
}

size_t	Post::findNextBoundary(size_t _bs)
{
	return (_body->find(_boundary, _bs + _boundary.size()));
}

bool Post::parseMultiPartHeader(std::string multiHeader)
{
	size_t fileNamePos = multiHeader.find("filename=\"");
	if (fileNamePos == std::string::npos)
		return (true);
	fileNamePos += 10;
	size_t endFileNamePos = multiHeader.find('\"', fileNamePos);
	if (endFileNamePos == std::string::npos)
		return (true);
	_fileName = multiHeader.substr(fileNamePos, endFileNamePos - fileNamePos);
	if (_contentLength < _serv->getMaxBodySize())
		_file.open(('.' + _serv->getFilePathPost() + _fileName).c_str(), std::ios::binary);
	return (false);
}
// yes i know it doesnt create a new file if the name already exists
// or it doesnt create a file if there is no filename

void	Post::writeInFile(std::string& content)
{
	if (_file.is_open() && _contentLength < _serv->getMaxBodySize())
	{
		_file.write(content.c_str(), content.size());
		_file.close();
	}
	else
		std::cerr << "Server couldn 't open file " << _fileName << std::endl;
}

// Premiere regle a noter si on ne croise pas le boundaryend on continue a lire
// mais 
bool	Post::handleMultipart()
{
	std::string boundaryEnd(_boundary + "--\r\n");

	static size_t	boundary_start = std::string::npos;
	size_t			_headerMultipartPos;
	boundary_start = (boundary_start == std::string::npos) ? _body->find(_boundary) : boundary_start;
	if (boundary_start == std::string::npos)
		return (false);

	while (_body->compare(boundary_start, boundaryEnd.size(), boundaryEnd))
	{
		_headerMultipartPos = _body->find("\r\n\r\n", boundary_start);
		if (_headerMultipartPos == std::string::npos)
			return (false);
		_headerMultipartPos += 4;

		size_t nextBoundaryPos = findNextBoundary(boundary_start);
		if (nextBoundaryPos == std::string::npos)
			return (false);
		std::string multipartContent = _body->substr(_headerMultipartPos, nextBoundaryPos - _headerMultipartPos - 2);
		if (!parseMultiPartHeader(_body->substr(boundary_start + _boundary.size(), _headerMultipartPos - (boundary_start + _boundary.size()))))
			writeInFile(multipartContent);
		boundary_start = nextBoundaryPos;
	}
	boundary_start = std::string::npos;
	return (true);
}

bool	Post::parseBody() {
	if (_contentType == "application/x-www-form-urlencoded")
	{
		_queryString = *_body;
	}
	else if (!_contentType.compare(0, 30 , "multipart/form-data; boundary="))
	{
		_boundary = "--" + _contentType.substr(30);
		return handleMultipart();
	}
	return true;
}

void	Post::parseHeader()
{
	for (std::vector<std::string>::iterator it = _head.begin(); it != _head.end()
												/*|| (!_contentType.empty()
													&& _contentLength != 0)*/ ; it++) {
		if (!it->compare(0, 13, "Content-Type:"))
		{
			_contentType = it->substr(14);
			_boundary = _contentType.substr(30);
		}
		if (!it->compare(0, 15, "Content-Length:")) {
			std::stringstream	convert;
			convert << it->substr(16);
			convert >> _contentLength;
		}
	}
}

bool	Post::virtualParse(void) {
	if (_code == CODE_METHOD_NOT_ALLOWED)
		return 1;
	if (_contentType.empty() && _contentLength == 0)
		parseHeader();

	if (!_contentType.empty())
	{
		return (parseBody());
	}
	return (1);
}



