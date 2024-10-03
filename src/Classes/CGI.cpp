/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mparisse <mparisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 00:04:02 by mparisse          #+#    #+#             */
/*   Updated: 2024/05/14 20:37:57 by mparisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"
#include "Method.hpp"
#include "webserv.hpp"
#include "RequestHandler.hpp"
#include "fileManagement.hpp"

CGI::CGI(cgi_type _type,Method *_method, RequestHandler *req, int epfd): AHandler(-1, req->getServerInfo(), epfd) {
	(void) _type;
	link[0] = 0;
	link[1] = 0;
	_env["QUERY_STRING"] = _method->getQueryString();
	link[0] = 0;
	link[1] = 0;
	_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	_env["QUERY_STRING"] = _method->getQueryString();
	_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	_env["REDIRECT_STATUS"] = "200";
	_env["HTTP_HOST"] = (_info->getServerDataName() + ':' + MessageContext::intToString(_info->getPort()));
	_env["SCRIPT_NAME"] = _method->getFileToOpen();
	_env["SERVER_NAME"] = _info->getServerDataName();
	_env["PATH_INFO"] = _method->getFileToOpen();
	_env["CONTENT_LENGTH"] = MessageContext::intToString(_method->getContentLength());
	_env["HTTP_USER_AGENT"] = _method->getUserAgent();

	if (_type == PHP)
		_pathCgi = "/usr/bin/php";
	if (_type == PYTHON)
		_pathCgi = "/usr/bin/python3";

	method_type ty = _method->getType();
	if (ty == HTTP_GET)
		_env["REQUEST_METHOD"] = "GET";
	else if (ty == HTTP_POST)
		_env["REQUEST_METHOD"] = "POST";
	else if (ty == HTTP_DELETE)
		_env["REQUEST_METHOD"] = "DELETE";
	_fileToOpen = _method->getFileToOpen();
	_reqPtr = req;
	_socketFd = req->getSocketFd();
	// method needs directly a query string
}

/*
doc: https://www.ibm.com/docs/en/netcoolomnibus/8.1?topic=scripts-environment-variables-in-cgi-script
*/
// void	CGI::initCGI(cgi_type _type, Method* _method) {

// }

CGI& CGI::operator=(const CGI& rhs) {
	_pathCgi = rhs._pathCgi;
	_fileToOpen = rhs._fileToOpen;
	_fileToOpen = rhs._fileToOpen;
	_env = rhs._env; // not deep copy but doesnt matter
	fdToSend = rhs.fdToSend;
	return (*this);
}

int	CGI::getReadOutPipe(void) {
	return (link[0]);
}

char	**mapToEnv(std::map<std::string, std::string> _env) {
	int		size = _env.size();
	char **	charArray = new char*[size + 1];

	charArray[size] = 0;
	int i = 0;
	for (std::map<std::string, std::string>::iterator it = _env.begin(); it != _env.end(); ++it) {
		charArray[i] = new char[it->first.length() + it->second.length() + 2]; // +2 for '=' and '\0'
		std::strcpy(charArray[i], (it->first + "=" + it->second).c_str());
		i++;
	}
	return charArray;
}

e_httpCode	CGI::execute(void) {
	int		forkstate;

	int		fd = open(_fileToOpen.c_str(), O_RDONLY);
	if (fd == -1) {
		return (CODE_INTERNAL_SERVER_ERROR);
	}
	if (pipe(link)) {
		perror("CGI");
		return (CODE_INTERNAL_SERVER_ERROR);
	}
	// 
	if (setNonBlockFd(link[0]))
		return (CODE_INTERNAL_SERVER_ERROR);

	forkstate = fork();
	if (forkstate < 0)
		return (CODE_INTERNAL_SERVER_ERROR);	
	if (forkstate == 0) {
		dup2(link[1], STDOUT_FILENO);
		close(link[1]);
		close(link[0]);
		close(_socketFd);
		close(_epollFd);
		char **			env = mapToEnv(_env);
		const char *	cmd[3] = {_pathCgi.c_str(), _fileToOpen.c_str(), 0};
		execve(cmd[0], (char **)cmd, env);
		exit(1);
	}
	int	status;
	waitpid(forkstate, &status, 0);
	if (WIFEXITED(status))
		status =  WEXITSTATUS(status);
		
	close(link[1]);
	close(fd);
	if (status)
	{
		close(link[0]);
		std::cout << "north side" << std::endl;
		return (CODE_INTERNAL_SERVER_ERROR);
	}
	return (CODE_OK);
}

int	CGI::handleRequest(int epollFd, epoll_event *ev)
{
	std::string _bodyFromCgi;
	ssize_t red;
	char buf[MAX_PIPE + 1];
	while (1)
	{
		red = read(link[0], buf, MAX_PIPE);
		if (red < 0)
			std::cout << "ERROR" << std::endl;
		if (red == 0)
			break ;
		buf[red] = 0;
		_bodyFromCgi.append(buf, red);
	}
	_reqPtr->setBodyFromCGI(_bodyFromCgi);
	epoll_ctl(epollFd, EPOLL_CTL_DEL, link[0], ev);
	close(link[0]);
	ev->events = EPOLLOUT;
	ev->data.ptr = _reqPtr;
	epoll_ctl(epollFd, EPOLL_CTL_MOD, _socketFd, ev);
	return 0;
}
