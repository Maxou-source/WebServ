

// #include "Func.hpp"
#include "webserv.hpp"
#include "FunctionWrapper.hpp"
#include "Parse.hpp"

int	Parse::checkFileName(const char *arg)
{
	std::string argStr = arg;
	if (argStr.size() < 5)
		return (0);
	if (argStr.substr(argStr.size() - 4) != ".cfg")
		return (0);
	// checking the extension fileName
	return (1);
}

int	Parse::checkFileRights(const char *arg, std::ifstream *argFile)
{
	argFile->open(arg);
	if (!argFile->is_open())
	{
		return (0);
	}
	return (1);
}

int	Parse::parseContent(std::ifstream *argFile, std::vector<ServerData *>& vectorServerData)
{
	std::string line;
	std::vector<std::string> vectorFile;
	while (std::getline(*argFile, line))
	{
		line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
		vectorFile.push_back(line);
	}
	// getting all the file in lines

	std::vector<FunctionWrapper> funcWrap;
	FunctionWrapper::buildFunctionWrapper(funcWrap);

	vecstr tmp;

	for (tmp = vectorFile.begin(); tmp != vectorFile.end(); tmp++)
	{
		for (; tmp != vectorFile.end() && tmp->empty(); tmp++)
			;
		if (*tmp !=  "ServerData{")
			return (0);
		tmp++;
		// checking that ServerData is written at the beggining

		// initialising funcWrap that will parse individually every configuration

		ServerData *currentServerData = new ServerData();
		// initializing current serv to add if correct

		size_t sze;
		std::string tmpString = *tmp;
		// initialzing temporary stuff for parsing

		while (*tmp != "}" && tmp != vectorFile.end())
		{
			funcParser2 func = 0;
			for (sze = 0; sze < funcWrap.size() && !tmpString.empty() && tmpString != "}"; sze++)
			{
				size_t sizeName = funcWrap[sze].getSizeName();
				std::string functionName = funcWrap[sze].getFunctionName();
				if (!tmpString.compare(0, sizeName, functionName)
					&& functionName != "location->"
					&& tmpString.at(tmpString.size() - 1) == ';')
				{
					*tmp = tmpString.substr(sizeName, tmpString.size() - sizeName - 1);
					func = funcWrap[sze].getFunction2();
				}
				else if (!tmpString.compare(0, sizeName, functionName) && functionName == "location->")
				{
					*tmp = tmpString.substr(sizeName, tmpString.size() - sizeName);
					func = funcWrap[sze].getFunction2();
				}
			}
			if (!func || func(currentServerData, tmp))
				{delete currentServerData; return (0);}
			tmp++;
			tmpString = *tmp;
		}
		if (*tmp != "}")
		{
			delete currentServerData;
			return (0);
		}
		vectorServerData.push_back(currentServerData);
	}
	return (1);
}

int	Parse::parse(const char *arg, std::vector<ServerData *>& vectorServerData)
{
	if (!checkFileName(arg))
	{
		std::cout << "KO: Something is wrong with the file name" << std::endl;
		return (1);
	}
	std::ifstream argFile;
	if (!checkFileRights(arg, &argFile))
	{
		std::cout << "KO: Something is wrong with the file rights or file doesn't exist" << std::endl;
		return (1);
	}

	if (!parseContent(&argFile, vectorServerData))
	{
		std::cout << "KO: Something is wrong with the syntax" << std::endl;
		return (1);
	}
	argFile.close();
	std::cout << "Everything is Fine!!" << std::endl;
	return (0);
}
