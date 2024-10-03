/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mparisse <mparisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 11:45:34 by mparisse          #+#    #+#             */
/*   Updated: 2024/05/15 08:33:59 by mparisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int main(int ac, char **av)
{
	Whole whole;
	std::string file;

	if (ac != 2)
	{
		std::cout << "Something went wrong setting -> using basic config file" << std::endl;
		file = "./config/basic.cfg";
	}
	else {
		file = av[1];
	}
	try {
		if (Parse::parse(file.c_str(), whole.getVectorServerData()))
			return (EXIT_FAILURE);
	}
	catch (const std::exception& e)
	{
		std::cout << "what() " << e.what() << std::endl;
		std::cout << "BIG_ERROR :: " << "Something went wrong with the parsing" << std::endl;
		return (EXIT_FAILURE);
	}
	printVectorServerData(whole.getVectorServerData());
	try {
		if (whole.initServerDatas())
		{
			std::cout << "ERROR ::" << " Couldn't setup a server" << std::endl;
			return (EXIT_FAILURE);
		}
		if (whole.initEpollInstances())
		{
			std::cout << "Epoll Error" << std::endl;
			return (EXIT_FAILURE);
		}
	}
	catch (const std::exception& e)
	{
		std::cout << "what() " << e.what() << std::endl;
		std::cout << "BIG_ERROR :: " << "Something went wrong with the servers setup" << std::endl;
		return (EXIT_FAILURE);
	}
	try {
		if (whole.startServerDatas())
			std::cout << "start server error" << std::endl;
	}
	catch (std::exception & e)
	{
		std::cout << "what() " << e.what() << std::endl;
		std::cout << "BIG_ERROR :: " << "Something went wrong during the running of the servers" << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

// default erro page send if problem with directory

// default config file

// do the correction subject 

// organize shit in var/web/

// what should i return when i had an upload (done)

// finish multipart (might have still an error case)

// finish cgi (done)

// delete method (done)

// directory listing (done)

// mbs (done)

// then you can do ctrl c
// freeeeeeee

// do all tests with curl

// and your done