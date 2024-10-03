/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mparisse <mparisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 01:04:11 by mparisse          #+#    #+#             */
/*   Updated: 2024/05/15 01:06:17 by mparisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Delete.hpp"

Delete::Delete(std::vector<std::string>& vec, std::string *_body, ServerData* serv)
: Method(vec, _body, serv) {
	_type = HTTP_DELETE;
}

bool	Delete::virtualParse()
{
	std::ifstream file;
	file.open(_fileToOpen.c_str());
	if (file.good())
	{
		if (std::remove(_fileToOpen.c_str()) != 0)
			_code = CODE_INTERNAL_SERVER_ERROR;
		else
			_code = CODE_OK;
		file.close();
	}
	else
	{
		_code = CODE_NOT_FOUND;
	}
	return (true);
}