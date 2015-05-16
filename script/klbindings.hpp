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

	protected: using KLSENTRY = double (*)(KLVariables&);	//!< Prototyp funkcji skryptowej.

	public: class KLBinding
	{

		protected:

			KLSENTRY Pointer;	//!< Adres zbindowanej funkcji.

		public:

			/*! \brief		Konstruktor kopiujący.
			 *  \param [in]	Binding Obiekt do skopiowania.
			 *
			 * Tworzy obiekt na bazie istniejącego.
			 *
			 */
			KLBinding(const KLBinding& Binding);

			/*! \brief		Operator konwersji z `KLSENTRY`.
			 *  \param [in]	Entry Adres funkcji do przypisania.
			 *
			 * Przypisuje podany adres do funkcji.
			 *
			 */
			KLBinding(KLSENTRY Entry);

			/*! \brief		Aktualizacja przypisania.
			 *  \param [in]	Entry Adres funkcji do przypisania.
			 *
			 * Przypisuje nowy adres do funkcji.
			 *
			 */
			void Update(KLSENTRY Entry);

			/*! \brief		Wywołanie funkcji.
			 *  \param [in]	Variables System zmiennych.
			 *  \return		Zwrócona wartość.
			 *
			 * Wywołuje funkcję z podanymi parametrami.
			 *
			 */
			double operator() (KLVariables& Variables);

	};

	protected:

		KLMap<KLBinding, KLString> Bindings;	//!< Kontener na przypisania.

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

		/*! \brief		Test obecności przypisania.
		 *  \param [in]	Name Nazwa przypisania.
		 *  \return		Obecność przypisania w systemie.
		 *
		 * Sprawdza czy przypisanie o podanej nazwie jest obecne w systemie.
		 *
		 */
		bool Exists(const KLString& Name) const;

		/*! \brief		Pobranie ilości przypisań.
		 *  \return		Ilośc przypisań.
		 *
		 * Sprawdza ile przypisań istnieje w systemie.
		 *
		 */
		int Size(void) const;

		/*! \brief		Operator wyboru.
		 *  \param [in]	Name Nazwa przypisania.
		 *  \return		Referencja do obiektu przypisania.
		 *  \warning		Gdy podane przypisanie nie istnieje zostają zastosowane reguły `KLMap::operator[]`.
		 *
		 * Wybiera z systemu przypisanie o podanej nazwie.
		 *
		 */
		KLBinding& operator[] (const KLString& Name);

		/*! \brief		Operator wyboru.
		 *  \param [in]	Name Nazwa przypisania.
		 *  \return		Stała referencja do obiektu przypisania.
		 *  \warning		Gdy podane przypisanie nie istnieje zostają zastosowane reguły `KLMap::operator[]`.
		 *
		 * Wybiera z systemu przypisanie o podanej nazwie.
		 *
		 */
		const KLBinding& operator[] (const KLString& Name) const;

		KLMap<KLBinding, KLString>::KLMapVarIterator begin(void);
		KLMap<KLBinding, KLString>::KLMapVarIterator end(void);

		KLMap<KLBinding, KLString>::KLMapConstIterator begin(void) const;
		KLMap<KLBinding, KLString>::KLMapConstIterator end(void) const;

};

#endif // KLBINDINGS_HPP
