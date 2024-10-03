

#include "Connexion.hpp"
#include "RequestHandler.hpp"

Connexion::~Connexion() {
	
}

int Connexion::handleRequest(int epollFd, epoll_event *ev)
{
	int	clientFd;

	clientFd = accept(getSocketFd(), 0, 0);

	setNonBlockFd(clientFd);
	RequestHandler *reqHandler = new RequestHandler(clientFd, getServerInfo(), this, _epollFd);
	std::cout << B_GREEN << "Connexion created on socket " << clientFd << RESET << std::endl; 
	ev->events = EPOLLIN;
	ev->data.ptr = reqHandler;
	epoll_ctl(epollFd, EPOLL_CTL_ADD, clientFd, ev);
	return (0);
}
