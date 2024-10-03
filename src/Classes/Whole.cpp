/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Whole.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mparisse <mparisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 00:27:43 by mparisse          #+#    #+#             */
/*   Updated: 2024/05/15 08:31:31 by mparisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Whole.hpp"
#include "ServerData.hpp"
#include "Connexion.hpp"


Whole::Whole() {
	_continue = true;
	_epollFd = -1;
}

Whole::~Whole()
{
	if (_epollFd != -1)
		close(_epollFd);
	for (std::vector<AHandler *>::iterator it = _requestHandler.begin(); it != _requestHandler.end(); it++)
		delete *it;
	_requestHandler.clear();
	for (std::vector<ServerData *>::iterator it = _vectorServerData.begin(); it != _vectorServerData.end(); it++)
	{
		int _sf = (*it)->getSocketFd();
		if (_sf != -1)
			close(_sf);
		delete *it;
	}
	_vectorServerData.clear();
}

void	Whole::stop(int) {
	_continue = false;
}

int Whole::initServerDatas()
{
	std::vector<ServerData*>::iterator	itserv;

	for (itserv = _vectorServerData.begin(); itserv != _vectorServerData.end(); itserv++)
	{
		if ((*itserv)->setup())
			return 1;
	}
	return (0);
}

int Whole::initEpollInstances(void)
{
	_epollFd = epoll_create(1);
	if (_epollFd < 0)
		return (1);
	for (std::vector<ServerData *>::iterator it = _vectorServerData.begin(); it != _vectorServerData.end() ; it++)
	{
		struct epoll_event	tmp;
		Connexion			*tmpRequest = new Connexion((*it)->getSocketFd(), *it, _epollFd);

		tmp.events = EPOLLIN;
		tmp.data.ptr = tmpRequest;
		_requestHandler.push_back(tmpRequest);
		epoll_ctl(_epollFd, EPOLL_CTL_ADD, tmpRequest->getSocketFd(), &tmp);
		_events.push_back(tmp);
	}
	return (0);
}

int Whole::startServerDatas(void)
{
	signal(SIGINT, Whole::stop);
	signal(SIGPIPE, SIG_IGN);
	while (_continue) {
		int nb_ready;
		nb_ready = epoll_wait(_epollFd, _events.data(), _vectorServerData.size(), -1);

		for (int i = 0; i < nb_ready; i++)
		{
			AHandler * m = static_cast<AHandler *>(_events[i].data.ptr);
			if (m->handleRequest(_epollFd, &_events[i]))
				return 1;
			if (m->getToFree())
			{
				delete m;
			}
		}
	}
	return 0;
}

bool Whole::_continue = true;

std::vector<ServerData *>&	Whole::getVectorServerData(void) {return _vectorServerData;}
void						Whole::setVectorServerData(std::vector<ServerData *> & vs) {_vectorServerData = vs;}

int							Whole::getEpollFd(void) {return _epollFd;};
void						Whole::setEpollFd(int ef) {_epollFd = ef;}

std::vector<epoll_event>	Whole::getEpollEvents(void) {return _events;}
