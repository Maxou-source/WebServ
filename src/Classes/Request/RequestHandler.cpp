/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mparisse <mparisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 00:24:06 by mparisse          #+#    #+#             */
/*   Updated: 2024/05/15 10:33:54 by mparisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "webserv.hpp"
# include "Get.hpp"
# include "Post.hpp"
# include "Delete.hpp"
# include "fileManagement.hpp"
# include "RequestHandler.hpp"
# include "MessageContext.hpp"

RequestHandler::RequestHandler(int new_clientFd, ServerData *Serverinfo, Connexion *c, int epfd)
: AHandler(new_clientFd, Serverinfo, epfd),
_connexion(c),
_requestLine(0),
_method(NULL),
_headRequest(0),
_bodyRequest(std::string()),
_codeCGI(CODE_OK),
_cgi_type(NO_CGI),
_cgi(NULL),
_notFinished(false) {}

RequestHandler::~RequestHandler() {
	
}

int			RequestHandler::getClientFd(void) {return _clientFd;}
void		RequestHandler::setClientFd(int clf) {_clientFd = clf;}


void	RequestHandler::setBodyFromCGI(std::string& _bR)  {_bodyFromCgi = _bR;}


int RequestHandler::getLineFromRequest(void)
{
	std::string					resString;
	char						buf[BUF_SIZE + 1];
	ssize_t						red = 0;

	red = read(_socketFd, buf, BUF_SIZE);
	if (red < 0)
	{
		std::cout << "exit recv" << std::endl;
		return (1);
	}
	if (red == 0)
		return 0;

	buf[red] = 0;
	resString = buf;
	size_t endHeader = resString.find("\r\n\r\n");
	if (_headRequest.size() < 1 && endHeader != std::string::npos)
	{
		std::string tmpHead;
		tmpHead = resString.substr(0, endHeader);
		_bodyRequest.append(&buf[endHeader + 4], red - (endHeader + 4));
		// weird split
		std::istringstream iss(tmpHead);
		std::string line;
		while (std::getline(iss, line, '\r')) {
			std::istringstream lineStream(line);
			std::string token;
			while (std::getline(lineStream, token, '\n'))
			{
				if (!token.empty())
					_headRequest.push_back(token);
			}
		}
		// weird split
	}
	else if (_headRequest.size() < 1 && endHeader == std::string::npos)
	{
		std::string tmpHead;
		tmpHead = resString.substr(0, endHeader);

		// weird split
		std::istringstream iss(tmpHead);
		std::string line;
		while (std::getline(iss, line, '\r')) {
			std::istringstream lineStream(line);
			std::string token;
			while (std::getline(lineStream, token, '\n'))
			{
				if (!token.empty())
					_headRequest.push_back(token);
			}
		}
		// weird split
	}
	else
	{
		_bodyRequest.append(buf, red);
	}
	return (0);
}



void RequestHandler::startHTPPMethod()
{
	if (_headRequest.empty())
	{
		_method = new Get(_headRequest, &_bodyRequest, _info);
		return ;
	}
	if (!_headRequest[0].compare(0, 3, MC_GET))
	{
		_method = new Get(_headRequest, &_bodyRequest, _info);
	}
	else if (!_headRequest[0].compare(0, 4, MC_POST))
	{
		_method = new Post(_headRequest, &_bodyRequest, _info);
	}
	else if (!_headRequest[0].compare(0, 6, MC_DELETE))
	{
		_method = new Delete(_headRequest, &_bodyRequest, _info);
	}
}

bool	RequestHandler::handleCgi()
{

	std::size_t foundPHP = _method->getFileToOpen().find(".php");
	std::size_t foundPython = _method->getFileToOpen().find(".py");

	if (foundPHP != std::string::npos) {
		_cgi = new CGI(PHP, _method, this, _epollFd);
		_cgi_type = PHP;
	}
	else if (foundPython != std::string::npos) {
		_cgi = new CGI(PYTHON, _method, this, _epollFd);
		_cgi_type = PYTHON;
	}
	else
		return false;
	_codeCGI = _cgi->execute();
	if (_codeCGI != CODE_OK)
		return false;
	return true;
}

int RequestHandler::readRequest(int epollFd, epoll_event *ev)
{
	if (getLineFromRequest()) // Recv HERE
		return 1;
	if (_notFinished)
		return 0;
	// printLines(_headRequest);
	try {
		if (!_method)
			startHTPPMethod();
		if (_method && !_method->virtualParse())
			return (0);
		
		if (_method && _method->parse() && handleCgi())
		{
			ev->events = EPOLLIN;
			ev->data.ptr = _cgi;
			epoll_ctl(epollFd, EPOLL_CTL_ADD, _cgi->getReadOutPipe(),ev);
			return 0;
		}
	}
	catch (std::exception & e)
	{
		std::cout << "INTERNAL SERVER ERROR" << std::endl;
		_codeCGI = CODE_INTERNAL_SERVER_ERROR;
	}
	ev->events = EPOLLOUT;
	epoll_ctl(epollFd, EPOLL_CTL_MOD, _socketFd, ev);
	return (0);
}

int RequestHandler::sendResponse(int epollFd, epoll_event *ev)
{
	std::string response_str;
	printLines(_headRequest);
	if (_cgi_type != NO_CGI)
		response_str = MessageContext::responseHeader(_codeCGI, true) + "Content-Length: " + 
					MessageContext::intToString(_bodyFromCgi.size()) + "\r\n\r\n" +_bodyFromCgi;
	else if (_method)
		response_str = _method->response();
	else
		response_str = MessageContext::errorPage(CODE_INTERNAL_SERVER_ERROR);
	if (send(_socketFd, response_str.c_str(), response_str.size(), 0) == -1) {
		delete _method;
		delete _cgi;
		return (1);
	}

	std::cout << B_GREEN << "correctly sent " << RESET << '\n' << std::endl;	 // DEBUG
	// std::cout << "response \n" << response_str << std::endl;

	close(_socketFd);
	epoll_ctl(epollFd, EPOLL_CTL_DEL, _socketFd, NULL);
	ev->events = EPOLLIN;
	ev->data.ptr = _connexion;
	delete _cgi;
	delete _method;
	return (0);
}

int RequestHandler::handleRequest(int epollFd, epoll_event *ev)
{
	if (ev->events == EPOLLIN) {
		readRequest(epollFd, ev);
		return (0);
	}
	else if (ev->events == EPOLLOUT) {
		sendResponse(epollFd, ev);
		_toFree = true;
		return 0;
	}
	return (1);
}
// need to check if the method is supported by the server
// need to check if the path (on localhost) exists (in another praticall way)


// std::string	Response::errorBody(int error_code) {

// 	return ("<h1> " + intToString(error_code) + " " + reasonPhrase(error_code) + " </h1>");
// }

