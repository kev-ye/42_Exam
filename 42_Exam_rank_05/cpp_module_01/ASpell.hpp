/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASpell.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaye <kaye@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 15:54:47 by kaye              #+#    #+#             */
/*   Updated: 2021/09/09 19:58:40 by kaye             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASPELL_HPP
# define ASPELL_HPP

#include "ATarget.hpp"
#include <iostream>

class ATarget;

class ASpell {
	public:
		ASpell(void);
		ASpell(std::string const & name, std::string const & effects);
		ASpell(ASpell const & src);
		virtual ~ASpell(void);

		ASpell & operator=(ASpell const & rhs);

	public:
		std::string const & getName(void) const;
		std::string const & getEffects(void) const;
		void launch(ATarget const & target);

		virtual ASpell *clone(void) const = 0;

	protected:
		std::string _name;
		std::string _effects;
};

#endif