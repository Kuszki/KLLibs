/***********************************************************************
 *
 * Lightweight List interpretation for KLLibs
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

/*! \file		kllist.hpp
 *  \brief	Deklaracje dla klasy KLList i jej składników.
 *
 */

/*! \file		kllist.cpp
 *  \brief	Implementacja klasy KLList i jej składników.
 *
 */

#ifndef KLLIST_HPP
#define KLLIST_HPP

/*! \brief	Lekka interpretacja listy.
 *  \tparam	Data Typ przechowywanych danych.
 *  \warning	Brak możliwości kopiowania listy.
 *  \todo		Możliwość kopiowania listy.
 *
 * Prosta i lekka interpretacja listy elementów. Lista jednokierunkowa bez wbudowanej obsługi kolejki i stosu.
 *
 */
template<typename Data>
class KLList
{

	/*! \brief		Struktura elementu listy.
	 *
	 * Przechowuje dane pojedynczego elementu listy i umożliwia usunięcie go.
	 *
	 */
	protected: struct KLListItem
	{

		KLListItem*	Next;	//!< Wskaźnik na kolejny element.
		Data*		Record;	//!< Wskaźnik na przechowywane dane.

		/*! \brief		Destruktor.
		 *
		 * Automatycznie usuwa przechowywane dane. Nie modyfikuje kolejnego elementu.
		 *
		 */
		~KLListItem(void);

	};

	protected: class KLListVarIterator
	{

		protected:

			KLListItem* Current;

		public:

			KLListVarIterator(KLListItem* Begin);

			Data& operator* (void);
			KLListVarIterator& operator++ (void);
			bool operator!= (const KLListVarIterator& Iterator) const;

	};

	protected: class KLListConstIterator
	{

		protected:

			KLListItem* Current;

		public:

			KLListConstIterator(KLListItem* Begin);

			const Data& operator* (void) const;
			KLListConstIterator& operator++ (void);
			bool operator!= (const KLListConstIterator& Iterator) const;

	};

	protected:

		KLListItem* Begin;	//!< Wskaźnik na początek listy.
		KLListItem* End;	//!< Wskaźnik na koniec listy.

		int Capacity;		//!< Liczba elementów listy.

	public:

		KLList(const KLList<Data>&) = delete;

		/*! \brief		Domyślny konstruktor.
		 *
		 * Inicjuje wszystkie pola obiektu.
		 *
		 */
		KLList(void);

		/*! \brief		Destruktor.
		 *
		 * Zwalnia wszystkie użyte zasoby.
		 *
		 */
		~KLList(void);

		/*! \brief		Wstawianie elementu.
		 *  \param [in]	Item Element dodawany do listy.
		 *  \return		Aktualna liczba elementów.
		 *
		 * Dodaje do listy kopie podanego elementu i zwraca nową ilość elementów.
		 *
		 */
		int Insert(const Data& Item);

		/*! \brief		Usunięcie elementu.
		 *  \param [in]	ID Indeks elementu numerowany od zera.
		 *  \return		Aktualna liczba elementów lub -1 w przypadku błędu.
		 *
		 * Usuwa wybrany element i zwraca aktualną ilość elementów. Gdy nie istnieje element o wybranym indeksie medoda zwróci -1.
		 *
		 */
		int Delete(int ID);

		/*! \brief		Sprawdzenie ilości elementów.
		 *  \return		Aktualna liczba elementów.
		 *
		 * Zwraca aktualną liczbę elementów.
		 *
		 */
		int Size(void) const;

		/*! \brief		Czyszczenie listy.
		 *
		 * Usuwa wszystkie elementy listy.
		 *
		 */
		void Clean(void);

		/*! \brief		Wybór elementu.
		 *  \param [in]	ID Indeks elementu.
		 *  \return		Referencja do wybranego elementu.
		 *  \warning		Gdy element o podanym indeksie nie istnieje to zwrócona zostanie niepoprawna referencja do `nullptr` co zapewne spowoduje krytyczny wyjątek.
		 *
		 * Wybiera element o podanym indeksie z listy.
		 *
		 */
		Data& operator[] (int ID);

		KLList& operator= (const KLList<Data>&) = delete;

		KLListVarIterator begin(void);
		KLListVarIterator end(void);

		KLListConstIterator begin(void) const;
		KLListConstIterator end(void) const;

};

#include "kllist.cpp"

#endif // KLLIST_HPP
