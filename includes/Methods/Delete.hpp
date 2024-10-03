/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mparisse <mparisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 01:01:05 by mparisse          #+#    #+#             */
/*   Updated: 2024/05/15 01:05:02 by mparisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DELETE_HPP
# define DELETE_HPP

# include "Method.hpp"

class Delete : public Method
{
	private:
	public:
		Delete(std::vector<std::string>& vec, std::string *_body, ServerData* servD);

		bool			virtualParse(void);
};

#endif