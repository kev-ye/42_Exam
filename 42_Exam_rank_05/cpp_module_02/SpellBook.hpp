/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SpellBook.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaye <kaye@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 18:14:15 by kaye              #+#    #+#             */
/*   Updated: 2021/09/09 20:25:22 by kaye             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPELLBOOK_HPP
# define SPELLBOOK_HPP

#include "ASpell.hpp"
#include "Fwoosh.hpp"
#include "Fireball.hpp"
#include "Polymorph.hpp"
#include <iostream>
#include <exception>
#include <vector>
#include <iterator>

class SpellBook {
	public:
		SpellBook(void);
		virtual ~SpellBook(void);

	private:
		SpellBook(SpellBook const & src);
		SpellBook & operator=(SpellBook const & rhs);

	public:
		void	learnSpell(ASpell *spell);
		void	forgetSpell(std::string const & spellName);
		ASpell*	createSpell(std::string const & spellName);

	private:
		std::vector<ASpell*> _spell;
};

#endif