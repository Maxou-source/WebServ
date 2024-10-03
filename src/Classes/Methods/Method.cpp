/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Method.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mparisse <mparisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 10:53:49 by mparisse          #+#    #+#             */
/*   Updated: 2024/05/15 09:56:12 by mparisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Method.hpp"
#include "MessageContext.hpp"
#include "Get.hpp"
#include "ServerData.hpp"
#include "Location.hpp"
#include "fileManagement.hpp"
#include "webserv.hpp"

/*======== CONSTRUCTORS ========*/

Method::Method() {
}

Method::~Method() {
}

std::string	Method::exctractDataFromURIAndCut(std::string& old_uri) {
	std::size_t	queryPos = old_uri.find('?');
	if (queryPos == std::string::npos) {
		return old_uri.substr(1);
	}
	_queryString = old_uri.substr(1);
	return (old_uri.substr(0, queryPos));
}

bool	Method::verifiedIfAllowed(const char *meth) {
	std::vector<std::string> allowedMethods = _serv->getMethods();
	std::vector<std::string>::iterator it;

	for (it = allowedMethods.begin(); it != allowedMethods.end(); it++) {
		if (*it == meth) {
			return (true);
		}
	}
	return (false);
}

// _code = (_code == CODE_UNDEFINED) ? CODE_OK : _code;
Method::Method(std::vector<std::string>& vec, std::string *_bodyRequest, ServerData *servD)
: _type(HTTP_UNDEFINED), _head(vec), _body(_bodyRequest), _serv(servD),
_code(CODE_OK), _contentLength(0)
{
	std::vector<std::string>	subString;
	std::istringstream			iss;
	std::string					token;
	std::string					response_str;

	if (_head.empty())
	{_code = CODE_BAD_REQUEST; return ;}
	iss.str(_head[0]);
	while (std::getline(iss, token, MC_SEPARATOR)) {
		subString.push_back(token);
	}
	if (!subString[1].compare("/")) {
		_pathName = "/";
		_fileToOpen = _serv->getRoutesName() + _serv->getIndex();
	}
	else {
		std::vector<Location *> loc = _serv->getLocations();
		if (loc.empty()) {
			_fileToOpen = _serv->getRoutesName() + exctractDataFromURIAndCut(subString[1]);
			_pathName = subString[1];
			return ;
		}
		std::vector<Location *>::iterator it;
		std::string name;
		for (it = loc.begin(); it != loc.end(); it++) {
			name = (*it)->getName();
			if (!subString[1].compare(0, subString[1].size(), name)) {
				break ;
			}
		}
		subString[1] = exctractDataFromURIAndCut(subString[1]);
		if (it == loc.end()) {
			_fileToOpen = _serv->getRoutesName() + subString[1];
			_pathName = subString[1];
			return ;
		}
		_serv = (*it);
		if (!verifiedIfAllowed(subString[0].c_str())){
			_code = CODE_METHOD_NOT_ALLOWED; return ;
		}
		_fileToOpen = _serv->getRoutesName();
		if (name.length() < subString[1].length())
		{
			_fileToOpen += subString[1].substr(name.length());
		}
		else
			_fileToOpen += _serv->getIndex();
		_pathName = subString[1];
	}
	
}

bool	Method::parse()
{
	for (std::vector<std::string>::iterator it = _head.begin(); it != _head.end(); it++) {
		if (!it->compare(0, 6, "Host: "))
		{
			_hostName = it->substr(6);
		}
		if (!it->compare(0, 11, "User-Agent:"))
		{
			_userAgent = it->substr(11);
		}
		if (!it->compare(0, 11, "Connection:"))
		{
			_connection = it->substr(11);
		}
	}
	if (MessageContext::isError(_code))
	{
		return (false);
	}
	if (_contentLength > _serv->getMaxBodySize()) {
		_code = CODE_CONTENT_TOO_LARGE; return false;
	}
	if (_hostName.empty())
	{
		_code = CODE_BAD_REQUEST;
		return (false);
	}
	if (!_serv->getRedirect().empty())
		_code = CODE_MOVED_PERMANENTLY;
	return (true);
}

std::string	Method::response()
{
	if (MessageContext::isError(_code))
		return (MessageContext::errorPage(_code));
	else if (MessageContext::isRedirection(_code))
		return MessageContext::redirection(_code, _serv);
	if (_serv->getIndex().empty() && _serv->getDirectoryListing())
		return (MessageContext::directoryListing(_fileToOpen));
	if (MessageContext::isADirectory(_fileToOpen))
	{
		std::string errorContent(MessageContext::getInstance().getErrorContent());
		return MessageContext::responseHeader(CODE_FORBIDDEN, true) +
							"Content-Length: " + MessageContext::intToString(errorContent.size()) + "\r\n\r\n" +
							errorContent;
	}
	std::string res;
	res = fileToString(_fileToOpen);
	if (res.empty() && _type != HTTP_DELETE)
		return MessageContext::errorPage(CODE_NOT_FOUND);
	return (MessageContext::responseHeader(_code, true) + "Content-Length: " + MessageContext::intToString(res.size()) + "\r\n\r\n" + res);
}

std::string	Method::getPathName() const {
	return (_pathName);
}

void		Method::setPathName(std::string const & newName) {
	_pathName = newName;
}

std::string Method::getHostName(void) const {
	return (_hostName);
}

void Method::setHostName(std::string const & newHostName) {
	_hostName = newHostName;
}

const std::string&	Method::getFileToOpen() const {
	return (_fileToOpen);
}

void		Method::setFileToOpen(std::string const & new_file) {
	_fileToOpen = new_file;
}

method_type	Method::getType() const {
	return (_type);
}
void		Method::setType(method_type new_type) {
	_type = new_type;
}

const std::string &	Method::getQueryString(void) const {
	return (_queryString);
}

void	Method::setQueryString(std::string & rhs) {
	_queryString = rhs;
}

ssize_t					Method::getContentLength() const {
	return _contentLength;
}

void					Method::setContentLength(ssize_t cl) {
	_contentLength = cl;
}

std::string				&Method::getUserAgent() {
	return _userAgent;
}

void					Method::setUserAgent(std::string & ua) {
	_userAgent = ua;
}

std::string				&Method::getConnection() {
	return _connection;
}
void					Method::setConnection(std::string& c) {
	_connection = c;
}