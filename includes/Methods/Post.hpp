/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mparisse <mparisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 18:55:57 by mparisse          #+#    #+#             */
/*   Updated: 2024/05/14 18:55:58 by mparisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef POST_HPP
# define POST_HPP

# include "Method.hpp"

enum e_contentTypeData
{
	MULTIPART,
	APPLICATION,
};

class Post : public Method
{
	private:
		std::string		_contentType;
		int				_clientFd;


		std::string		_boundary;
		std::string		_fileName;
		std::ofstream	_file;
	public:
		Post() {}
		Post(std::vector<std::string>& vec, std::string *_body ,ServerData* servD);
		Post(std::vector<std::string>& vec, std::string *_body ,ServerData* servD, bool);

		std::string		virtualResponse(void);
		bool			virtualParse(void);

		bool			parseMultiPartHeader(std::string);
		size_t			findNextBoundary(size_t _bs);
		void			writeInFile(std::string& content);

		bool			handleMultipart();
		void			parseHeader();
		bool			parseBody();
};


#endif
