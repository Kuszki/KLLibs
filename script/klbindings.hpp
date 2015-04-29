/***********************************************************************
 *
 * Function binding management system interpretation for KLLibs
 * Copyright (C) 2015  Łukasz "Kuszki" Dróżdż
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 **********************************************************************/

#ifndef KLBINDINGS_HPP
#define KLBINDINGS_HPP

/*! \file		klbindings.hpp
 *  \brief	Deklaracje dla klasy KLBindings i jej składników.
 *
 */

/*! \file		klbindings.cpp
 *  \brief	Implementacja klasy KLBindings i jej składników.
 *
 */

#ifdef QT_VERSION
	#include "kllibs.hpp"
#else
	#define EXPORT
#endif

#include "containers/klmap.hpp"
#include "containers/klstring.hpp"

#include "script/klvariables.hpp"

/*! \brief	System bindowania funkcji dla skryptu.
 *
 * System zarządzania bindowaniem funkcji umożliwiający przypisywanie etykiety funkcją o podanym prototypie i wywoływanie ich w skrypcie.
 *
 */
class KLBindings
{

	protected:

		using KLSENTRY = void (*)(KLVariables&);	//!< Prototyp funkcji skryptowej.

		KLMap<KLSENTRY, KLString> Bindings;		//!< Kontener na przypisania.

	public:

		/*! \brief		Dodawanie przypisania.
		 *  \param [in]	Name		Nazwa przypisania.
		 *  \param [in]	Entry	Adres funkcji do przypisania.
		 *  \return		Powodzenie operacji.
		 *
		 * Dodaje do systemu nową funkcję o podanym adresie.
		 *
		 */
		bool Add(const KLString& Name,
			    KLSENTRY Entry);

		/*! \brief		Usuwanie przypisania.
		 *  \param [in]	Name		Nazwa przypisania.
		 *  \return		Powodzenie operacji.
		 *
		 * Usuwa przypisanie z systemu.
		 *
		 */
		bool Delete(const KLString& Name);

		/*! \brief		Aktualizacja przypisania.
		 *  \param [in]	Name		Nazwa przypisania.
		 *  \param [in]	Entry	Adres funkcji do przypisania.
		 *  \return		Powodzenie operacji.
		 *
		 * Przypisuje nowy adres do podanej funkcji.
		 *
		 */
		bool Update(const KLString& Name,
				  KLSENTRY Entry);

		/*! \brief		Wywołanie funkcji.
		 *  \param [in]	Name		Nazwa przypisania.
		 *  \param [in]	Scoope	System zmiennych.
		 *  \return		Powodzenie operacji.
		 *
		 * Wywołuje funkcje o podanej nazwie.
		 *
		 */
		bool Call(const KLString& Name,
				KLVariables& Scoope);

		/*! \brief		Test obecności przypisania.
		 *  \param [in]	Name Nazwa przypisania.
		 *  \return		Obecność przypisania w systemie.
		 *
		 * Sprawdza czy przypisanie o podanej nazwie jest obecne w systemie.
		 *
		 */
		bool Exists(const KLString& Name) const;

};

#endif // KLBINDINGS_HPP
