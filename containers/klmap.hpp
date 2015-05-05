/***********************************************************************
 *
 * Lightweight Map interpretation for KLLibs
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

#ifndef KLMAP_HPP
#define KLMAP_HPP

/*! \file		klmap.hpp
 *  \brief	Deklaracje dla klasy KLMap i jej składników.
 *
 */

/*! \file		klmap.cpp
 *  \brief	Implementacja klasy KLMap i jej składników.
 *
 */

/*! \brief	Lekka interpretacja listy.
 *  \tparam	Data	Typ przechowywanych danych.
 *  \tparam	Key	Typ używanego klucza.
 *  \note		Do użycia wymagany jest konstruktor kopiujący dla klucza i danych.
 *
 * Prosta i lekka interpretacja mapy elementów. Wspiera możliwość iteracji po zakresie.
 *
 */
template<typename Data, typename Key>
class KLMap
{

	/*! \brief		Struktura reprezentująca parę klucz-dane.
	 *
	 * Struktura przechowująca informacje o obiekcie przechowywanym w mapie.
	 *
	 */
	public: struct KLMapRecord
	{

		Data	Value;	//!< Dane obiektu.
		Key	ID;		//!< Klucz obiektu.

		/*! \brief		Konstruktor rekordu.
		 *  \param [in]	_Value	Dane rekordu.
		 *  \param [in]	_ID		Klucz rekordu.
		 *
		 * Tworzy nowy rekord na podstawie podanych obiektów klucza i danych. Kopiuje wszystkie obiekty.
		 *
		 */
		KLMapRecord(const Data& _Value, const Key& _ID);

	};

	/*! \brief		Struktura elementu mapy.
	 *
	 * Przechowuje dane pojedynczego elementu listy i umożliwia usunięcie go.
	 *
	 */
	protected: struct KLMapItem
	{

		KLMapItem*	Next;	//!< Wskaźnik na kolejny element.
		KLMapRecord*	Record;	//!< Wskaźnik na dane elementu.

		/*! \brief		Destruktor.
		 *
		 * Automatycznie usuwa przechowywane dane. Nie modyfikuje kolejnego elementu.
		 *
		 */
		~KLMapItem(void);

	};

	public: class KLMapVarIterator
	{

		protected:

			KLMapItem* Current;

		public:

			KLMapVarIterator(KLMapItem* Begin);

			KLMapRecord& operator* (void);
			KLMapVarIterator& operator++ (void);
			bool operator!= (const KLMapVarIterator& Iterator) const;

	};

	public: class KLMapConstIterator
	{

		protected:

			KLMapItem* Current;

		public:

			KLMapConstIterator(KLMapItem* Begin);

			const KLMapRecord& operator* (void) const;
			KLMapConstIterator& operator++ (void);
			bool operator!= (const KLMapConstIterator& Iterator) const;

	};

	protected:

		KLMapItem* Begin;	//!< Wskaźnik na początek mapy.
		KLMapItem* End;	//!< Wskaźnik na koniec mapy.

		int Capacity;		//!< Liczba elementów mapy.

	public:

		/*! \brief		Konstruktor kopiujący.
		 *  \param [in]	Map Mapa do sklonowania.
		 *
		 * Klonuje wybraną instancje mapy.
		 *
		 */
		KLMap(const KLMap<Data, Key>& Map);

		/*! \brief		Konstruktor przenoszący.
		 *  \param [in]	Map Mapa do przeniesienia.
		 *
		 * Przenosi wybraną instancje mapy.
		 *
		 */
		KLMap(KLMap<Data, Key>&& Map);

		/*! \brief		Domyślny konstruktor.
		 *
		 * Inicjuje wszystkie pola obiektu.
		 *
		 */
		KLMap(void);

		/*! \brief		Destruktor.
		 *
		 * Zwalnia wszystkie użyte zasoby.
		 *
		 */
		~KLMap(void);

		/*! \brief		Wstawianie elementu.
		 *  \param [in]	Item	Element dodawany do listy.
		 *  \param [in]	ID	Identyfikator obiektu.
		 *  \return		Aktualna liczba elementów.
		 *
		 * Dodaje do listy kopie podanego elementu i zwraca nową ilość elementów.
		 *
		 */
		int Insert(const Data& Item,
				 const Key& ID);

		/*! \brief		Usunięcie elementu.
		 *  \param [in]	ID Klucz elementu.
		 *  \return		Aktualna liczba elementów lub -1 w przypadku błędu.
		 *
		 * Usuwa wybrany element i zwraca aktualną ilość elementów. Gdy nie istnieje element o wybranym indeksie medoda zwróci -1.
		 *
		 */
		int Delete(const Key& ID);

		/*! \brief		Test klucza.
		 *  \param [in]	ID Klucz do sprawdzenia.
		 *  \return		`true` jeśli element o podanym kluczu istnieje, lub `false` gdy nie iestnieje.
		 *
		 * Sprawdza czy obiekt o podanym kluczu istnieje i zwraca odpowiednią wartość.
		 *
		 */
		bool Exists(const Key& ID) const;

		/*! \brief		Zmiana klucza obiektu.
		 *  \param [in]	OldID	Klucz do zamiany.
		 *  \param [in]	NewID	Nowy klucz.
		 *  \return		Powodzenie operacji.
		 *
		 * Zamienia podany klucz na nowy. Zwraca `true` gdy udało się zamienić klucz, lub `false` gdy nie znaleziono podanego klucza.
		 *
		 */
		bool Update(const Key& OldID,
				  const Key& NewID);

		/*! \brief		Sprawdzenie ilości elementów.
		 *  \return		Aktualna liczba elementów.
		 *
		 * Zwraca aktualną liczbę elementów.
		 *
		 */
		int Size(void) const;

		/*! \brief		Czyszczenie mapy.
		 *
		 * Usuwa wszystkie elementy mapy.
		 *
		 */
		void Clean(void);

		/*! \brief		Wybór elementu.
		 *  \param [in]	ID Klucz elementu.
		 *  \return		Referencja do wybranego elementu.
		 *  \warning		Gdy element o podanym kluczu nie istnieje to zwrócona zostanie niepoprawna referencja do `nullptr` co zapewne spowoduje krytyczny wyjątek.
		 *
		 * Wybiera element o podanym kluczu z mapy.
		 *
		 */
		Data& operator[] (const Key& ID);

		/*! \brief		Wybór elementu.
		 *  \param [in]	ID Klucz elementu.
		 *  \return		Stała referencja do wybranego elementu.
		 *  \warning		Gdy element o podanym kluczu nie istnieje to zwrócona zostanie niepoprawna referencja do `nullptr` co zapewne spowoduje krytyczny wyjątek.
		 *
		 * Wybiera element o podanym kluczu z mapy.
		 *
		 */
		const Data& operator[] (const Key& ID) const;

		/*! \brief		Operator przypisania.
		 *  \param [in]	Map Obiekt do sklonowania.
		 *  \return		Referencja do bierzącego obiektu.
		 *
		 * Zwalnia dotychczasowe zasoby i klonuje wybrany obiekt.
		 *
		 */
		KLMap<Data, Key>& operator= (const KLMap<Data, Key>& Map);

		KLMapVarIterator begin(void);
		KLMapVarIterator end(void);

		KLMapConstIterator begin(void) const;
		KLMapConstIterator end(void) const;

};

#include "klmap.cpp"

#endif // KLMAP_HPP
