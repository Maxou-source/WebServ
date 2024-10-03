

#include "webserv.hpp"

std::string	fileToString(const std::string& filename)
{
	std::ifstream	file;
	std::string		toOpen;

	try {
		toOpen = "./" + filename;
		file.open(toOpen.c_str());
		if (!file.is_open()) {
			std::cerr << "Failed to open the file: " << filename << std::endl;
			return ("");
		}
		std::string fileContent((std::istreambuf_iterator<char>(file)),
								std::istreambuf_iterator<char>());
		file.close();
		return (fileContent);
	}
	catch (const std::exception& e)
	{
		std::cout << "what() " << e.what() << std::endl;
		return(std::string());
	}
}

int	setNonBlockFd(int fd) {
	int	flags;

	flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1) {
		return (1);
	}
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
		return (1);
	}
	return (0);
}