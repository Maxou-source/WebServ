/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Method.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mparisse <mparisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 15:12:47 by mparisse          #+#    #+#             */
/*   Updated: 2024/05/15 01:05:45 by mparisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef METHOD_HPP
# define METHOD_HPP

# include <string>
# include <vector>
# include <iostream>
# include <fstream>
# include <sstream>

# include "MessageContext.hpp"


class ServerData;
class Location;

enum	method_type {
	HTTP_UNDEFINED,
	HTTP_GET,
	HTTP_POST,
	HTTP_DELETE
};


class Method
{
	protected:
		method_type					_type;
		std::vector<std::string>	_head;
		std::string					*_body;
		ServerData *				_serv;

		std::string					_pathName;
		std::string					_fileToOpen;
		Location *					_curentloc;
		std::string					_hostName;
		std::string					_userAgent;
		std::string					_connection;
		std::string					_queryString;

		e_httpCode					_code;
		ssize_t						_contentLength;

	public:
		Method();
		virtual ~Method();
		Method(std::vector<std::string>&, std::string*,ServerData *);
		Method(std::vector<std::string>&, std::string*,ServerData *, bool);

		std::string				getPathName() const;
		void					setPathName(std::string const & new_name);

		std::string				getHostName(void) const;
		void					setHostName(std::string const &);


		const std::string&		getFileToOpen() const;
		void					setFileToOpen(std::string const & new_file);

		method_type				getType() const;
		void					setType(method_type new_type);

		const std::string&		getQueryString(void) const;
		void					setQueryString(std::string&);

		ssize_t					getContentLength() const;
		void					setContentLength(ssize_t );

		std::string				&getUserAgent();
		void					setUserAgent(std::string &);

		std::string				&getConnection();
		void					setConnection(std::string&);

		virtual bool			virtualParse(void) = 0;
		bool					parse();
		std::string				response();
		std::string				exctractDataFromURIAndCut(std::string& old_uri);
		bool					verifiedIfAllowed(const char *meth);

};

#endif
