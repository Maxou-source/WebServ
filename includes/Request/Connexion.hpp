
#ifndef CONNEXION_HPP
# define CONNEXION_HPP

# include <string>
# include <vector>
# include <fstream>
# include <sys/epoll.h>
# include <sys/socket.h>
# include <sstream>
# include <stdlib.h>
# include "AHandler.hpp"

class Connexion : public AHandler
{
	public:
		~Connexion();
		int		handleRequest(int fd, epoll_event *);

		Connexion(int sf, ServerData *sd, int epfd) : AHandler(sf, sd, epfd) {}
};

#endif
