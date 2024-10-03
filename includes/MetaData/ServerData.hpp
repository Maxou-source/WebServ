/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerData.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mparisse <mparisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 22:10:49 by mparisse          #+#    #+#             */
/*   Updated: 2024/05/14 20:47:35 by mparisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERDATA_HPP
# define SERVERDATA_HPP

# include <dirent.h>
# include <sys/epoll.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <unistd.h>
# include <fcntl.h>
# include <vector>
# include <string>

# define BACKLOG 10

typedef struct sockaddr_in t_sock;

class Location;

class ServerData
{
	protected:
		std::string					_ip;
		unsigned int				_port;
		std::string					_ServerDataName;
		std::string					_index;
		ssize_t						_maxBodySize;
		DIR							*_routes;
		std::string					_routesName;
		std::vector<Location *>		_location;
		std::vector<std::string>	_methods;
		bool						_directory_listing;
		std::string					_redirect;
		std::string					_filepath_post;
		int							_socketFd;

	public:
		ServerData();
		~ServerData();
		t_sock						initSocks(void);

		void						setIp(std::string ip);
		std::string					getIp();

		void						setPort(int port);
		unsigned int				getPort();

		void						setServerDataName(std::string  ServerDataName) ;
		std::string					getServerDataName();

		void						setIndex(std::string index);
		std::string					getIndex();

		void						setMaxBodySize(unsigned int mbs);
		unsigned int				getMaxBodySize();

		DIR							*getRoutes();
		void						setRoutes(DIR *routes);
		std::string					getRoutesName();
		void						setRoutesName(std::string routesName);

		std::vector<std::string>	getMethods();
		void						setMethods(std::vector<std::string> &meth);

		std::vector<Location *>&	getLocations();
		void						setLocations(std::vector<Location *> l) ;
		void						addLocation(Location* location);
		static int					checkLocName(std::vector<Location *> loc, std::string & str);

		int							getSocketFd(void) const;
		void						setSocketFd(int s);

		std::string&				getRedirect(void);
		void						setRedirect(std::string& r);

		std::string&				getFilePathPost(void);
		void						setFilePathPost(std::string& fp);

		bool						getDirectoryListing(void);
		void						setDirectoryListing(bool dl);


		int							setup(void);

};

#endif
