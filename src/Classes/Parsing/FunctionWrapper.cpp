/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FunctionWrapper.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mparisse <mparisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 00:37:20 by mparisse          #+#    #+#             */
/*   Updated: 2024/05/15 10:31:48 by mparisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FunctionWrapper.hpp"
#include "webserv.hpp"

static int isValidIPAddress(const std::string& input) {
	int dotCount = 0;
	int numbersBetweenDots = 0;

	for (int i = 0; input[i]; i++) {
		if (input[i] == '.')
		{
			dotCount++;
			if (numbersBetweenDots > 3)
				return 0;
			numbersBetweenDots = 0;
		}
		else if (std::isdigit(input[i]))
			numbersBetweenDots++;
		else
			return 0;
		}
	return (dotCount == 3) && (numbersBetweenDots <= 3);
}

int FunctionWrapper::parseListen(ServerData* serv, vecstr &vec)
{
	std::string ve = *vec;
	std::string pad = ve.substr(0, ve.find(':'));
	if (!isValidIPAddress(pad))
		return 1;
	serv->setIp(pad);
	std::stringstream pad2(ve.substr(ve.find(':') + 1));
	try {
		unsigned int port;
		pad2 >> port;
		if (pad2.fail() || pad2.bad() || port > 65000)
			return 1;
		serv->setPort(port);
	}
	catch (const std::exception &e) {return 1;}
	return 0;
}

int FunctionWrapper::parseLocation(ServerData *serv, vecstr &vec)
{
	std::string ve = *vec;
	if (ve.at(ve.size() - 1) != '{' && ve[1] != '/')
		return 1;
	// std::cout << "[" << ve << std::endl;
	// verifying syntax

	Location *currentLocation = new Location(serv);

	std::string tmpName = ve.substr(0, ve.size() - 1);
	if (tmpName.empty() || ServerData::checkLocName(serv->getLocations(), tmpName))
	{
		serv->addLocation(currentLocation);
		return 1;
	}
	currentLocation->setName(tmpName);
	vec++;
	std::string tmpString = *vec;

	std::vector<FunctionWrapper> funcWrap;
	FunctionWrapper::buildFunctionWrapper(funcWrap);


	while (tmpString != "}" || tmpString.empty())
	{
		// std::cout << tmpString << std::endl;
		funcParser2 func = 0;
		size_t sze;
		for (sze = 0; sze < funcWrap.size() && !tmpString.empty() && tmpString != "}"; sze++)
		{
			size_t sizeName = funcWrap[sze].getSizeName();
			std::string functionName = funcWrap[sze].getFunctionName();
			if (!tmpString.compare(0, sizeName, functionName)
				&& functionName != "location->" && functionName != "listen->"
				&& functionName != "ServerData_name->" && functionName != "default_error_page->"
				&& tmpString.at(tmpString.size() - 1) == ';')
			{
				*vec = tmpString.substr(sizeName, tmpString.size() - sizeName - 1);
				func = funcWrap[sze].getFunction2();
			}
		}
		// not allowed listen, ServerData_name location of course
		if (!func || func(currentLocation, vec))
		{
			serv->addLocation(currentLocation);
			return 1;
		}
		vec++;
		tmpString = *vec;
	}
	// if (currentLocation->getRoutesName().empty())
	// 	std::cout << "name "<< currentLocation->getName() <<"  is empty" << std::endl;
	serv->addLocation(currentLocation);
	return 0;
}

int FunctionWrapper::parseServerDataName(ServerData * serv, vecstr& str)
{
	std::string ve = *str;
	serv->setServerDataName(ve);
	return 0;
}

int FunctionWrapper::parseIndex(ServerData * serv, vecstr& vec)
{
	std::string str = *vec;
	if (str.size() < 4)
		return 1;
	if (str.compare(str.length() - 4, 4, ".php")
		&& str.compare(str.length() - 5, 5, ".html") && str.compare(str.length() - 4, 4, ".css"))
	{
		return 1;
	}
	serv->setIndex(str);
	return 0;
}

int FunctionWrapper::parseMaxBodySize(ServerData * serv, vecstr& vec)
{
	std::string ve = *vec;
	std::stringstream ss(ve);
	ssize_t max_body_size;
	ss >> max_body_size;
	if (ss.fail() || ss.bad())
	{
		std::cout << "fail" << std::endl;
		return 1;
	}
	serv->setMaxBodySize(max_body_size);
	return 0;
}

int FunctionWrapper::parseRoutes(ServerData * serv, vecstr& vec)
{
	std::string str =*vec;
	const char *dirName = str.c_str();
	DIR *dir;

	if ((dir = opendir(dirName)) != NULL)
	{
		serv->setRoutes(dir);
		serv->setRoutesName(str);
		closedir(dir);
		return 0;
	}
	else
	{
		return 1;
	}
}

int FunctionWrapper::parseMethods(ServerData* serv, vecstr& vec)
{
	std::istringstream tokens(*vec);
	char delim = '|';
	std::vector<std::string> tokenMethods;
	std::string token;

	while (std::getline(tokens, token, delim)) {
		if (token != "GET" && token != "POST" && token != "DELETE")
			return 1;
		std::vector<std::string>::iterator wtv = std::find(tokenMethods.begin(), tokenMethods.end(), token);
		if (wtv != tokenMethods.end())
		{
			return 1;
		}
		tokenMethods.push_back(token);
	}
	if (tokenMethods.size() > 3)
		return 1;
	serv->setMethods(tokenMethods);
	return 0;
}

int	FunctionWrapper::parseRedirect(ServerData *serv, vecstr &vec)
{
	serv->setRedirect(*vec);
	return 0;
}

int	FunctionWrapper::parseDirectoryListing(ServerData* serv, vecstr &vec)
{
	std::string str = *vec; 
	if (str == "on")
	{
		serv->setDirectoryListing(true);
		return 0;
	}
	else if (str == "off")
	{
		serv->setDirectoryListing(false);
		return 0;
	}
	return 1;
}

int	FunctionWrapper::parseFilePathPost(ServerData* serv, vecstr &vec)
{
	std::string str = *vec;
	DIR* dir = opendir(('.' + str).c_str());
	if (dir != NULL) {
		closedir(dir);
		serv->setFilePathPost(str);
		return false;
	}
	return true;
}

int	FunctionWrapper::parseErrorPage(ServerData *serv, vecstr &vec)
{
	std::string str = *vec;
	std::ifstream	file;
	std::string		toOpen;

	(void) serv;
	try {
		toOpen = "./" + str;
		file.open(toOpen.c_str());
		if (!file.is_open()) {
			std::cerr << "Failed to open the file: " << str << std::endl;
			return (1);
		}
		std::string fileContent((std::istreambuf_iterator<char>(file)),
								std::istreambuf_iterator<char>());
		MessageContext::getInstance().setErrorContent(fileContent);
		return (0);
	}
	catch (const std::exception& e)
	{
		std::cout << "what() " << e.what() << std::endl;
		return(1);
	}
}

void FunctionWrapper::buildFunctionWrapper(std::vector<FunctionWrapper>& wrapper)
{
	FunctionWrapper listen("listen->", FunctionWrapper::parseListen);
	FunctionWrapper location("location->", FunctionWrapper::parseLocation);
	FunctionWrapper ServerDataName("ServerData_name->", FunctionWrapper::parseServerDataName);
	FunctionWrapper index("index->", FunctionWrapper::parseIndex);
	FunctionWrapper mbs("max_body_size->", parseMaxBodySize);
	FunctionWrapper routes("routes->", FunctionWrapper::parseRoutes);
	FunctionWrapper methods("allowed_methods->", FunctionWrapper::parseMethods);

	FunctionWrapper	redirect("redirect->", FunctionWrapper::parseRedirect);
	FunctionWrapper	filepath_post("filepath_post->", FunctionWrapper::parseFilePathPost);
	FunctionWrapper	directory_listing("directory_listing->", FunctionWrapper::parseDirectoryListing);
	FunctionWrapper errorPage("default_error_page->", FunctionWrapper::parseErrorPage);

	wrapper.push_back(listen);
	wrapper.push_back(location);
	wrapper.push_back(ServerDataName);
	wrapper.push_back(index);
	wrapper.push_back(mbs);
	wrapper.push_back(routes);
	wrapper.push_back(methods);

	wrapper.push_back(redirect);
	wrapper.push_back(filepath_post);
	wrapper.push_back(directory_listing);
	wrapper.push_back(errorPage);
}
