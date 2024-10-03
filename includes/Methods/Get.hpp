/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mparisse <mparisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 10:40:17 by mparisse          #+#    #+#             */
/*   Updated: 2024/05/06 21:11:02 by mparisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_HPP
# define GET_HPP

# include "Method.hpp"

class Get : public Method
{
	private:

	public:
		Get();
		Get(std::vector<std::string>& vec, std::string *_body, ServerData* servD);
		Get(std::vector<std::string>& vec, std::string *_body, ServerData* servD, bool);

		std::string		virtualResponse(void);
		bool			virtualParse(void);
};



#endif
