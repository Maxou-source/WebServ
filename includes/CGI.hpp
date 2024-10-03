/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mparisse <mparisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 21:48:44 by mparisse          #+#    #+#             */
/*   Updated: 2024/05/14 20:37:34 by mparisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include <map>
# include <string>
# include <errno.h>
# include <sys/wait.h>
# include "MessageContext.hpp"
# include "AHandler.hpp"

class Method;
class RequestHandler;

enum	cgi_type
{
	NO_CGI,
	PHP,
	PYTHON,
};

class CGI : public AHandler
{
	private:
		RequestHandler						*_reqPtr;
		std::string							_pathCgi;
		std::string							_fileToOpen;

		std::map<std::string, std::string>	_env;

		int									link[2];
		int									fdToSend;

	public:
		CGI(RequestHandler*);
		CGI(Method *);
		CGI(cgi_type, Method *);
		CGI(cgi_type, Method *, RequestHandler *, int epfd);
		CGI& operator=(const CGI& rhs);

		// void		initCGI(cgi_type _type, Method* _method);
		e_httpCode	execute();
		int			handleRequest(int fd, epoll_event *);

		int			getReadOutPipe(void);

};

#endif