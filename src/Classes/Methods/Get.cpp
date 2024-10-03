/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mparisse <mparisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 17:04:20 by mparisse          #+#    #+#             */
/*   Updated: 2024/05/15 01:06:51 by mparisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Get.hpp"
# include "fileManagement.hpp"
#include "ServerData.hpp"
#include "MessageContext.hpp"

Get::Get(std::vector<std::string>& vec, std::string *_body, ServerData* serv)
: Method(vec, _body, serv) {
	_type = HTTP_GET;
}

bool	Get::virtualParse(void) {
	_contentLength = -1;
	return (1);
}
