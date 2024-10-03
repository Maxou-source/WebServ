

#include "ServerData.hpp"
#include "Location.hpp"

ServerData::~ServerData() {
	for (std::vector<Location *>::iterator it = _location.begin(); it != _location.end(); it++)
		delete *it;
}

ServerData::ServerData() {
	_socketFd = -1;
	_maxBodySize = 0;
	_directory_listing = false;
}

int	ServerData::checkLocName(std::vector<Location *> loc, std::string & str)
{
	std::vector<Location *>::iterator it;
	for (it = loc.begin(); it != loc.end(); it++)
	{
		Location tmp = *(*it);
		if (tmp.getName() == str)
			return 1;
	}
	return 0;
}

t_sock ServerData::initSocks(void)
{
	t_sock sock;

	sock.sin_family = AF_INET;
	sock.sin_addr.s_addr = inet_addr(_ip.c_str());
	sock.sin_port = htons(_port);
	return (sock);
}

int	ServerData::setup(void)
{
	if ((_socketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP )) == -1)
		return (1);
	this->setSocketFd(_socketFd);

	t_sock sock = initSocks();

	// TODO RM
	int opt = 1;
	if (setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) == -1)
		return (close(_socketFd), 1);

	if (bind(_socketFd, (struct sockaddr *) & sock, sizeof(t_sock)) == -1)
		return (close(_socketFd), 1);
	// creating a socket for each ServerData and associating the port and ip
	// to it


	if (listen(_socketFd, 1) == -1)
		return (close(_socketFd), 1);
	// listening on the _socketFd the fucking fd

	int flags = fcntl(_socketFd, F_GETFL, 0);
	if (flags == -1)
		return (false);
	if (fcntl(_socketFd, F_SETFL, flags | O_NONBLOCK) == -1)
		return (1);
		// getting the flags of socketFd and adding nonblocking to it
		// NEED TO SET ERRORS DAMN
	return 0;
}

std::string					ServerData::getIp() {return _ip;}
void						ServerData::setIp(std::string ip) {_ip = ip;}

unsigned int				ServerData::getPort() {return _port;}
void						ServerData::setPort(int port) {_port = port;}

std::string					ServerData::getServerDataName() {return _ServerDataName;}
void						ServerData::setServerDataName(std::string  ServerDataName) {_ServerDataName = ServerDataName;}

std::string					ServerData::getIndex() {return _index;}
void						ServerData::setIndex(std::string index) {_index = index;}

unsigned int				ServerData::getMaxBodySize() {return _maxBodySize;}
void						ServerData::setMaxBodySize(unsigned int mbs) {_maxBodySize = mbs;}

DIR*						ServerData::getRoutes() {return _routes;};
void						ServerData::setRoutes(DIR *routes) {_routes = routes;}
std::string					ServerData::getRoutesName() {return _routesName;};
void						ServerData::setRoutesName(std::string routesName) {_routesName = routesName;}


std::vector<Location *>&	ServerData::getLocations() {return _location;}
void						ServerData::setLocations(std::vector<Location *> l) {_location = l;}
void						ServerData::addLocation(Location* location) {_location.push_back(location);}

int							ServerData::getSocketFd(void) const {return _socketFd;}
void						ServerData::setSocketFd(int s) {_socketFd = s;}

std::vector<std::string>	ServerData::getMethods() {return _methods;}
void						ServerData::setMethods(std::vector<std::string> &meth) {_methods = meth;}

std::string&				ServerData::getRedirect(void) {return _redirect;}
void						ServerData::setRedirect(std::string& r) {_redirect = r;}

std::string&				ServerData::getFilePathPost(void) {return _filepath_post;}
void						ServerData::setFilePathPost(std::string& fp) {_filepath_post = fp;}

bool						ServerData::getDirectoryListing(void) {return _directory_listing;}
void						ServerData::setDirectoryListing(bool dl) {_directory_listing = dl;}