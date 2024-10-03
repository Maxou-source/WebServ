/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageContext.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mparisse <mparisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 18:56:15 by mparisse          #+#    #+#             */
/*   Updated: 2024/05/15 10:08:43 by mparisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGECONTEXT_HPP
# define MESSAGECONTEXT_HPP

# include <string>
# include <sstream>
# include <stdlib.h>

# define B_GREEN "\033[1;32m"
# define B_RED "\033[0;31m"
# define B_BLUE "\033[1;36m"
# define B_YELLOW "\033[1;33m"
# define B_MAGENTA "\033[1;35m"

# define DEFAULT_PATH "/"
# define DEFAULT_ERROR_FILE "error.html"

# define MC_GET "GET"
# define MC_POST "POST"
# define MC_DELETE "DELETE"
# define MC_SEPARATOR ' '
# define PROTOCOL_VERSION "HTTP/1.1"

// rouge requete
// bleu reponse
// vert debug
# define RESET "\033[0m"
/*
* About those HTTP codes
*
* ========= 1xx - Informational =========
* Indicates an interim response for communicating connection status
* or request progress prior to completing the requested action and
* sending a final response.
*
* ========= 2xx - Successful =========
* Indicates that the client's request was successfully received,
* understood, and accepted.
*
* ========= 3xx - Redirection =========
* Indicates that further action needs to be taken by the user agent
* in order to fulfill the request.
*
* ========= 4xx - Client Error =========
* Indicates that the client seems to have erred.
*
* ========= 5xx - ServerData Error =========
* Indicates that the ServerData is aware that it has erred
* or is incapable of performing the requested method.
*
*/
enum e_httpCode
{
	CODE_UNDEFINED                       = 0,
	CODE_CGI                             = 1,
	CODE_CONTINUE                        = 100,
	CODE_SWITCHING_PROTOCOLS             = 101,
	CODE_PROCESSING                      = 102,
	CODE_EARLY_HINTS                     = 103,

	CODE_OK                              = 200,
	CODE_CREATED                         = 201,
	CODE_ACCEPTED                        = 202,
	CODE_NON_AUTHORITATIVE_INFORMATION   = 203,
	CODE_NO_CONTENT                      = 204,
	CODE_RESET_CONTENT                   = 205,
	CODE_PARTIAL_CONTENT                 = 206,
	CODE_MULTI_STATUS                    = 207,
	CODE_ALREADY_REPORTED                = 208,
	CODE_IM_USED                         = 226,

	CODE_MULTIPLE_CHOICES                = 300,
	CODE_MOVED_PERMANENTLY               = 301,
	CODE_FOUND                           = 302,
	CODE_SEE_OTHER                       = 303,
	CODE_NOT_MODIFIED                    = 304,
	CODE_USE_PROXY                       = 305,
	CODE_TEMPORARY_REDIRECT              = 307,
	CODE_PERMANENT_REDIRECT              = 308,

	CODE_BAD_REQUEST                     = 400,
	CODE_UNAUTHORIZED                    = 401,
	CODE_PAYMENT_REQUIRED                = 402,
	CODE_FORBIDDEN                       = 403,
	CODE_NOT_FOUND                       = 404,
	CODE_METHOD_NOT_ALLOWED              = 405,
	CODE_NOT_ACCEPTABLE                  = 406,
	CODE_PROXY_AUTHENTICATION_REQUIRED   = 407,
	CODE_REQUEST_TIMEOUT                 = 408,
	CODE_CONFLICT                        = 409,
	CODE_GONE                            = 410,
	CODE_LENGTH_REQUIRED                 = 411,
	CODE_PRECONDITIO_NFAILED             = 412,
	CODE_CONTENT_TOO_LARGE               = 413,
	CODE_PAYLOAD_TOO_LARGE               = 413,
	CODE_URI_TOO_LONG                    = 414,
	CODE_UNSUPPORTED_MEDIATYPE           = 415,
	CODE_RANGE_NOT_SATISFIABLE           = 416,
	CODE_EXPECTATION_FAILED              = 417,
	CODE_IM_A_TEAPOT                     = 418,
	CODE_MISDIRECTED_REQUEST             = 421,
	CODE_UNPROCESSABLE_CONTENT           = 422,
	CODE_UNPROCESSABLE_ENTITY            = 422,
	CODE_LOCKED                          = 423,
	CODE_FAILED_DEPENDENCY               = 424,
	CODE_TOO_EARLY                       = 425,
	CODE_UPGRADE_REQUIRED                = 426,
	CODE_PRECONDITION_REQUIRED           = 428,
	CODE_TOO_MANY_REQUESTS               = 429,
	CODE_REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
	CODE_UNAVAILABLE_FOR_LEGAL_REASONS   = 451,

	CODE_INTERNAL_SERVER_ERROR           = 500,
	CODE_NOT_IMPLEMENTED                 = 501,
	CODE_BAD_GATEWAY                     = 502,
	CODE_SERVICE_UNAVAILABLE             = 503,
	CODE_GATEWAY_TIMEOUT                 = 504,
	CODE_HTTP_VERSION_NOT_SUPPORTED      = 505,
	CODE_VARIANT_ALSO_NEGOTIATES         = 506,
	CODE_INSUFFICIENT_STORAGE            = 507,
	CODE_LOOP_DETECTED                   = 508,
	CODE_NOT_EXTENDED                    = 510,

	CODE_NETWORK_AUTHENTICATION_REQUIRED = 511,
};

class ServerData;

class MessageContext
{
	private:
		std::string _errorContent;
		MessageContext() {}
	public:
		static bool	isInformational(int code);
		static bool	isSuccessful(int code);
		static bool	isRedirection(int code);
		static bool	isClientError(int code);
		static bool	isServerDataError(int code);
		static bool	isError(int code);
		static std::string reasonPhrase(int code);
		static std::string	responseHeader(int code, bool);
		static std::string	errorPage(int _code);
		static std::string	intToString(ssize_t to_string);
		static std::string	redirection(int _code, ServerData *serv);
		static std::string	directoryListing(std::string&);
		static bool isADirectory(std::string& path);


		static MessageContext& getInstance();

		std::string	&getErrorContent();
		void		setErrorContent(std::string &);
};

#endif

