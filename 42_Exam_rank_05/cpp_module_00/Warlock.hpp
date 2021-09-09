/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Warlock.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaye <kaye@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 14:27:52 by kaye              #+#    #+#             */
/*   Updated: 2021/09/09 14:54:51 by kaye             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WARLOCK_HPP
# define WARLOCK_HPP

#include <iostream>

class Warlock {
	public:
		Warlock(std::string const & name, std::string const & title);
		virtual ~Warlock(void);

	private:
		Warlock(void);
		Warlock(Warlock const & src);
		Warlock & operator=(Warlock const & rhs);

	public:
		void				setTitle(std::string const & title);
		std::string const &	getName(void) const;
		std::string const &	getTitle(void) const;
		void				introduce(void) const;

	private:
		std::string _name;
		std::string _title;
};

#endif