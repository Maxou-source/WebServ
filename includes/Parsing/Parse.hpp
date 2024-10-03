/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mparisse <mparisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 14:48:03 by mparisse          #+#    #+#             */
/*   Updated: 2024/05/14 21:37:35 by mparisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_HPP
# define PARSE_HPP

class Parse
{
	public:
		static int	parse(const char *arg, std::vector<ServerData *>& vectorServerData);
		static int	checkFileRights(const char *arg, std::ifstream *argFile);
		static int	checkFileName(const char *arg);
		static int	parseContent(std::ifstream *argFile, std::vector<ServerData *>& vectorServerData);
};


#endif
