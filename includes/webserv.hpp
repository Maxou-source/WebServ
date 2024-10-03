/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mparisse <mparisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 11:45:27 by mparisse          #+#    #+#             */
/*   Updated: 2024/05/14 18:57:10 by mparisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
# include <fstream>
# include <string>
# include <stdlib.h>
# include <sstream>
# include <bitset>
# include <vector>
# include <typeinfo>
# include <algorithm>
# include <functional>
# include <dirent.h>
# include <cstring>
# include <sys/socket.h>
# include <netinet/in.h>
# include <fcntl.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <sys/types.h>
# include <stdio.h>
# include <unistd.h>
# include <climits>
# include <sys/epoll.h>
# include <signal.h>

# include "ServerData.hpp"
# include "Connexion.hpp"
# include "Location.hpp"
# include "Whole.hpp"
# include "Parse.hpp"
# include "fileManagement.hpp"
# include "MessageContext.hpp"

# define BUF_SIZE 10000
# define MAX_PIPE 65536

void		printVectorServerData(std::vector<ServerData *> &vec);
void		printServerData(ServerData* serv);
void		printLines(std::vector<std::string> line_to_read);

#endif
