/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mparisse <mparisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 22:32:40 by mparisse          #+#    #+#             */
/*   Updated: 2024/05/11 23:23:49 by mparisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "ServerData.hpp"

class Location : public ServerData
{
	private:
		ServerData		*_serv;
		std::string		_name;

	public:
		Location(ServerData *s) {_serv = s;}
		std::string		getName() {return _name;}
		void			setName(std::string na) {_name = na;}
};


#endif
