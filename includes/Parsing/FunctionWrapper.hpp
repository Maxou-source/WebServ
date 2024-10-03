/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FunctionWrapper.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mparisse <mparisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 00:40:35 by mparisse          #+#    #+#             */
/*   Updated: 2024/05/14 23:28:43 by mparisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

typedef std::vector<std::string>::iterator vecstr;

typedef int			(*funcParser)(ServerData& serv, std::string name);
typedef int			(*funcParser2)(ServerData* serv, std::vector<std::string>::iterator &ve);

class FunctionWrapper
{
	private:
		std::string	_functionName;
		funcParser	_function;
		funcParser2	_function2;
		size_t		_sizeName;

	public:
		FunctionWrapper(const std::string& name, funcParser func)
			:	_functionName(name), _function(func) {
				_sizeName = name.size();
			}

		FunctionWrapper(const std::string& name, funcParser2 func)
			:	_functionName(name), _function2(func) {
				_sizeName = name.size();
			}

		std::string&	getFunctionName(void) {return _functionName;}
		size_t			getSizeName(void) {return _sizeName;}
		funcParser		getFunction() {return _function;}
		funcParser2		getFunction2() {return _function2;}

		static void		buildFunctionWrapper(std::vector<FunctionWrapper>&);
		static int		parseListen(ServerData* serv, vecstr &);
		static int		parseLocation(ServerData *serv, vecstr &);
		static int		parseServerDataName(ServerData * serv, vecstr&);
		static int		parseIndex(ServerData * serv, vecstr& str);
		static int		parseMaxBodySize(ServerData * serv,vecstr& str);
		static int		parseRoutes(ServerData * serv, vecstr& str);
		static int		parseMethods(ServerData * serv, vecstr& str);

		static int		parseDirectoryListing(ServerData * serv, vecstr& str);
		static int		parseRedirect(ServerData * serv, vecstr& str);
		static int		parseFilePathPost(ServerData * serv, vecstr& str);
		static int		parseErrorPage(ServerData * serv, vecstr& str);
};

// int parseListen(ServerData& serv, std::string str);
// int parseServerDataName(ServerData & serv, std::string str);
