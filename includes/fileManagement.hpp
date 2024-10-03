/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileManagement.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mparisse <mparisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 18:55:37 by mparisse          #+#    #+#             */
/*   Updated: 2024/05/14 18:55:43 by mparisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_MANAGEMENT_HPP
# define FILE_MANAGEMENT_HPP

std::string	fileToString(const std::string& filename);
int			setNonBlockFd(int fd);

#endif