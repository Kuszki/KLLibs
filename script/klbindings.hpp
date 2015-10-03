/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Function binding management system interpretation for KLLibs           *
 *  Copyright (C) 2015  Łukasz "Kuszki" Dróżdż            l.drozdz@o2.pl   *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the  Free Software Foundation, either  version 3 of the  License, or   *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This  program  is  distributed  in the hope  that it will be useful,   *
 *  but WITHOUT ANY  WARRANTY;  without  even  the  implied  warranty of   *
 *  MERCHANTABILITY  or  FITNESS  FOR  A  PARTICULAR  PURPOSE.  See  the   *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have  received a copy  of the  GNU General Public License   *
 *  along with this program. If not, see http://www.gnu.org/licenses/.     *
 *                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef KLBINDINGS_HPP
#define KLBINDINGS_HPP

#include "../libbuild.hpp"

#include "../containers/klmap.hpp"
#include "../containers/klstring.hpp"

#include "klvariables.hpp"

#if defined(USING_BOOST)
#include <boost/function.hpp>
#include <boost/bind.hpp>
#endif

/*! \file		klbindings.hpp
 *  \brief	Deklaracje dla klasy KLBindings i jej składników.
 *
 */

/*! \file		klbindings.cpp
 *  \brief	Implementacja klasy KLBindings i jej składników.
 *
 */

/*! \brief	System bindowania funkcji dla skryptu.
 *
 * System zarządzania bindowaniem funkcji umożliwiający przypisywanie etykiety funkcją o podanym prototypie i wywoływanie ich w skrypcie.
 *
 */
class KLLIBS_EXPORT KLBindings
{

#if defined(USING_BOOST)
	public: using KLSENTRY = boost::function<double (KLVariables&)>;
#else
	public: using KLSENTRY = double (*)(KLVariables&);
#endif

	/*! \brief		Reprezentacja pojedynczego bindu.
	 *
	 * Definiuje bind przechowywany w organizacji.
	 *
	 */
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
		bool Add(const KLString& Name, KLSENTRY Entry);

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
