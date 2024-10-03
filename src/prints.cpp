/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prints.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mparisse <mparisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 14:02:36 by mparisse          #+#    #+#             */
/*   Updated: 2024/05/15 10:14:59 by mparisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	printLocations(std::vector<Location *> locs)
{
	std::vector<Location *>::iterator	itvec;
	for (itvec = locs.begin(); itvec != locs.end(); itvec++)
	{
		Location serv = *(*itvec);
		std::cout << B_GREEN << "  ServerData Location : " << RESET << std::endl;
		std::cout << B_MAGENTA << "      Location addr   : " << RESET << *itvec << std::endl;
		std::cout << B_MAGENTA << "      Location name   : " << RESET << serv.getName() << std::endl;
		std::cout << B_MAGENTA << "      Location routes : " << RESET << serv.getRoutesName() << std::endl;
		std::cout << B_MAGENTA << "      Location index  : " << RESET << serv.getIndex() << std::endl;
		std::cout << B_MAGENTA << "      Location mbs    : " << RESET << serv.getMaxBodySize() << std::endl;
		std::cout << B_MAGENTA << "      Allowed Meth :" << RESET;
		std::vector<std::string> tmp = serv.getMethods();
		for (std::vector<std::string>::iterator it = tmp.begin(); it != tmp.end(); it++)
		{
			std::cout << " " << *it;
		}
		std::cout << std::endl;
	}
}

void	printServerData(ServerData* serv)
{
	std::cout << B_GREEN << "  ServerData addr : " << RESET << &serv << std::endl;
	std::cout << B_GREEN << "  ServerData ip : " << RESET << serv->getIp() << std::endl;
	std::cout << B_GREEN << "  ServerData port : " << RESET << serv->getPort() << std::endl;
	std::cout << B_GREEN << "  ServerData name : " << RESET << serv->getServerDataName() << std::endl;
	std::cout << B_GREEN << "  ServerData index : " << RESET << serv->getIndex() << std::endl;
	std::cout << B_GREEN << "  ServerData mbs : " << RESET << serv->getMaxBodySize() << std::endl;
	std::cout << B_GREEN << "  ServerData routes : " << RESET << serv->getRoutesName() << std::endl;
	std::cout << B_GREEN << "  Allowed Meth :" << RESET;
	std::vector<std::string> tmp = serv->getMethods();
	for (std::vector<std::string>::iterator it = tmp.begin(); it != tmp.end(); it++)
	{
		std::cout << " " << *it;
	}
	std::cout << std::endl;
	printLocations(serv->getLocations());

}

void	printVectorServerData(std::vector<ServerData *> &vec)
{
	for (std::vector<ServerData *>::iterator it = vec.begin(); it != vec.end(); it++)
	{
		printServerData(dynamic_cast<ServerData *>(*it));
	}
	std::cout << std::endl;
}

void	printLines(std::vector<std::string> line_to_read) {
	std::vector<std::string>::iterator it;

	std::cout << "REQUEST HEADER IS";
	std::cout << B_YELLOW << std::endl;
	for (it = line_to_read.begin(); it != line_to_read.end(); it++) {
		std::cout << *it << std::endl;
	}
	std::cout << RESET << std::flush;
}
