/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Lightweight List interpretation for KLLibs                             *
 *  Copyright (C) 2015  Łukasz "Kuszki" Dróżdż  l.drozdz@openmailbox.org   *
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

#ifndef KLLIST_HPP
#define KLLIST_HPP

#include "../libbuild.hpp"

/*! \file		kllist.hpp
 *  \brief	Deklaracje dla klasy KLList i jej składników.
 *
 */

/*! \file		kllist.cpp
 *  \brief	Implementacja klasy KLList i jej składników.
 *
 */

/*! \brief	Lekka interpretacja listy.
 *  \tparam	Data Typ przechowywanych danych.
 *
 * Prosta i lekka interpretacja listy elementów. Lista jednokierunkowa z wbudowaną obsługą kolejki i stosu.
 *
 */
template<typename Data>
class KLList
{

	/*! \brief		Wyliczenie indeksu listy.
	 *
	 * Umożliwia indeksowanie listy z użyciem pierwszego i ostatniego elementu.
	 *
	 */
	public: enum INDEX : int
	{
		FIRST = 0,	//!< Pierwszy element.
		LAST = -1		//!< Ostatni element.
	};

	/*! \brief		Struktura elementu listy.
	 *
	 * Przechowuje dane pojedynczego elementu listy i umożliwia usunięcie go.
	 *
	 */
	protected: struct KLListItem
	{

		KLListItem*	Next;	//!< Wskaźnik na kolejny element.
		Data*		Record;	//!< Wskaźnik na przechowywane dane.

		/*! \brief		Domyślny konstruktor.
		 *
		 * Inicjuje wszystkie pola obiektu.
		 *
		 */
		KLListItem(void);

		/*! \brief		Destruktor.
		 *
		 * Automatycznie usuwa przechowywane dane. Nie modyfikuje kolejnego elementu.
		 *
		 */
		~KLListItem(void);

	};

	public: class KLListVarIterator
	{

		protected:

			KLListItem* Current;

		public:

			KLListVarIterator(KLListItem* Begin);

			Data& operator* (void);
			KLListVarIterator& operator++ (void);
			bool operator!= (const KLListVarIterator& Iterator) const;

	};

	public: class KLListConstIterator
	{

		protected:

			const KLListItem* Current;

		public:

			KLListConstIterator(const KLListItem* Begin);

			const Data& operator* (void) const;
			KLListConstIterator& operator++ (void);
			bool operator!= (const KLListConstIterator& Iterator) const;

	};

	protected:

		KLListItem* Begin;	//!< Wskaźnik na początek listy.
		KLListItem* End;	//!< Wskaźnik na koniec listy.

		int Capacity;		//!< Liczba elementów listy.

	public:

		/*! \brief		Konstruktor kopiujący.
		 *  \param [in]	List Lista do sklonowania.
		 *
		 * Klonuje wybraną instancje listy.
		 *
		 */
		KLList(const KLList<Data>& List);

		/*! \brief		Konstruktor przenoszący.
		 *  \param [in]	List Lista do przeniesienia.
		 *
		 * Przenosi wybraną instancje listy.
		 *
		 */
		KLList(KLList<Data>&& List);

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

		/*! \brief		Pobranie elementu.
		 *  \return		Kolejny element.
		 *
		 * Pobiera element w trybie kolejki i usuwa go z listy.
		 *
		 */
		Data Dequeue(void);

		/*! \brief		Pobranie elementu.
		 *  \return		Kolejny element.
		 *
		 * Pobiera element w trybie stosu i usuwa go z listy.
		 *
		 */
		Data Pop(void);

		/*! \brief		Wybór pierwszego elementu.
		 *  \return		Referencja do pierwszego elementu.
		 *  \warning		Gdy element o podanym indeksie nie istnieje to zwrócona zostanie niepoprawna referencja do `nullptr` co zapewne spowoduje krytyczny wyjątek.
		 *
		 * Wybiera pierwszy element z listy.
		 *
		 */
		Data& First(void);

		/*! \brief		Wybór pierwszego elementu.
		 *  \return		Stała referencja do pierwszego elementu.
		 *  \warning		Gdy element o podanym indeksie nie istnieje to zwrócona zostanie niepoprawna referencja do `nullptr` co zapewne spowoduje krytyczny wyjątek.
		 *
		 * Wybiera pierwszy element z listy.
		 *
		 */
		const Data& First(void) const;

		/*! \brief		Wybór ostatniego elementu.
		 *  \return		Referencja do ostatniego elementu.
		 *  \warning		Gdy element o podanym indeksie nie istnieje to zwrócona zostanie niepoprawna referencja do `nullptr` co zapewne spowoduje krytyczny wyjątek.
		 *
		 * Wybiera ostatni element z listy.
		 *
		 */
		Data& Last(void);

		/*! \brief		Wybór ostatniego elementu.
		 *  \return		Stała referencja do ostatniego elementu.
		 *  \warning		Gdy element o podanym indeksie nie istnieje to zwrócona zostanie niepoprawna referencja do `nullptr` co zapewne spowoduje krytyczny wyjątek.
		 *
		 * Wybiera ostatni element z listy.
		 *
		 */
		const Data& Last(void) const;

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

		/*! \brief		Wybór elementu.
		 *  \param [in]	ID Indeks elementu.
		 *  \return		Stała referencja do wybranego elementu.
		 *  \warning		Gdy element o podanym indeksie nie istnieje to zwrócona zostanie niepoprawna referencja do `nullptr` co zapewne spowoduje krytyczny wyjątek.
		 *
		 * Wybiera element o podanym indeksie z listy.
		 *
		 */
		const Data& operator[] (int ID) const;

		/*! \brief		Operator wejścia.
		 *  \param [in]	Item Obiekt do dodania.
		 *  \return		Referencja do bierzącego obiektu.
		 *
		 * Dodaje wybrany element na koniec listy.
		 *
		 */
		KLList<Data>& operator<< (const Data& Item);

		/*! \brief		Operator przypisania.
		 *  \param [in]	List Obiekt do sklonowania.
		 *  \return		Referencja do bierzącego obiektu.
		 *
		 * Zwalnia dotychczasowe zasoby i klonuje wybrany obiekt.
		 *
		 */
		KLList<Data>& operator= (const KLList<Data>& List);

		/*! \brief		Operator przeniesienia.
		 *  \param [in]	List Obiekt do przeniesienia.
		 *  \return		Referencja do bierzącego obiektu.
		 *
		 * Zwalnia dotychczasowe zasoby i przenosi wybrany obiekt.
		 *
		 */
		KLList<Data>& operator= (KLList<Data>&& List);

		KLListVarIterator begin(void);
		KLListVarIterator end(void);

		KLListConstIterator begin(void) const;
		KLListConstIterator end(void) const;

};

#include "kllist.cpp"

#endif // KLLIST_HPP
