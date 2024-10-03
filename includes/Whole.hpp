/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Whole.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mparisse <mparisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 00:28:21 by mparisse          #+#    #+#             */
/*   Updated: 2024/05/14 19:00:46 by mparisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WHOLE_HPP
# define WHOLE_HPP

# include <vector>
# include <string>
# include <sys/epoll.h>
# include "AHandler.hpp"
# include "ServerData.hpp"

class Whole
{
	private:
		std::vector<ServerData *>	_vectorServerData;
		int							_epollFd;
		std::vector <epoll_event>	_events;
		std::vector <AHandler *>	_requestHandler;

		static bool						_continue;

	public:
		Whole();
		~Whole();
		std::vector<ServerData *>&	getVectorServerData(void);
		void						setVectorServerData(std::vector<ServerData *> & vs);

		int							getEpollFd(void);
		void						setEpollFd(int ef);

		std::vector<epoll_event>	getEpollEvents(void);

		int							initServerDatas(void);
		int							initEpollInstances(void);
		int							startServerDatas(void);
		static void						stop(int);
};

#endif
