#ifndef AHANDLER_HPP
# define AHANDLER_HPP

# include <iostream>
# include <string>
# include <sys/epoll.h>

# include "ServerData.hpp"

class AHandler
{
	protected:
		int				_socketFd;
		ServerData*		_info;
		int				_epollFd;

		bool			_toFree;
	public:
		virtual ~AHandler() { }
		// AHandler(): _socketFd(-1), _info(NULL), _toFree(false) { }
		AHandler(int sf, ServerData *sd, int epfd): _socketFd(sf), _info(sd), _epollFd(epfd), _toFree(false) {}

		int				getSocketFd(void) {return _socketFd;}
		void			setSocketFd(int sf) {_socketFd = sf;}

		ServerData*		getServerInfo() {return _info;}
		void			setServerInfo(ServerData *sd) {_info = sd;}

		bool			getToFree(void) const {return _toFree;}
		void			setToFree(bool tf) {_toFree = tf;}

		virtual int		handleRequest(int, epoll_event *) = 0;
};

#endif
