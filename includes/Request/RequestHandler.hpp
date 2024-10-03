/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mparisse <mparisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 14:03:45 by mparisse          #+#    #+#             */
/*   Updated: 2024/05/14 20:36:40 by mparisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

# define BUFFER_SIZE 10

# include <string>
# include <vector>
# include <fstream>
# include <sys/epoll.h>
# include <sys/socket.h>
# include <sstream>
# include <stdlib.h>
# include "webserv.hpp"
# include "AHandler.hpp"
# include "Method.hpp"
# include "CGI.hpp"

typedef std::vector<std::string>::iterator vecstr;

class Connexion;

class RequestHandler : public AHandler
{
	private:
		int							_clientFd;
		Connexion *					_connexion;
		std::vector<std::string>	_requestLine;
		Method *					_method;

		std::vector<std::string>	_headRequest;
		std::string					_bodyRequest;

		e_httpCode					_codeCGI;
		std::string					_bodyFromCgi;

		cgi_type					_cgi_type;
		CGI							*_cgi;
		bool						_notFinished;
		// std::string					_response_str;
		// TEMPORARY !!!!!!!!!!!!!

	public:
		/*=== Constructor ===*/
		RequestHandler(int new_clientFd, ServerData *new_Serverinfo);
		RequestHandler(int, ServerData*, Connexion *, int epfd);
		~RequestHandler();

		/*=== Setters && Getters ===*/
		int								getClientFd(void);
		void							setClientFd(int clf);

		/*=== Methods ===*/
		int								readRequest(int, epoll_event*);
		int								sendResponse(int, epoll_event*);
		int								handleRequest(int fd, epoll_event *);

		bool							verifiedIfAllowed(const char *meth);
		void							startHTPPMethod();
		bool							handleCgi();

		int								getLineFromRequest(void);

		void							setBodyFromCGI(std::string& br);
};

std::string errorPage(int _code);
#endif
