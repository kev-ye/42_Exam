/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ATarget.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaye <kaye@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 16:23:33 by kaye              #+#    #+#             */
/*   Updated: 2021/09/09 19:58:45 by kaye             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ATARGET_HPP
# define ATARGET_HPP

#include "ASpell.hpp"
#include <iostream>

class ASpell;

class ATarget {
	public:
		ATarget(void);
		ATarget(std::string const & type);
		ATarget(ATarget const & src);
		virtual ~ATarget(void);

		ATarget & operator=(ATarget const & rhs);

	public:
		std::string const & getType(void) const;
		void getHitBySpell(ASpell const & spell) const;
	
		virtual ATarget *clone(void) const = 0;

	protected:
		std::string _type;	
};

#endif