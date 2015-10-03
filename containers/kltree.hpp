/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Lightweight Tree interpretation for KLLibs                             *
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

#ifndef KLTREE_HPP
#define KLTREE_HPP

#include "../libbuild.hpp"

/*! \file		kltree.hpp
 *  \brief	Deklaracje dla klasy KLTree i jej składników.
 *
 */

/*! \file		kltree.cpp
 *  \brief	Implementacja klasy KLTree i jej składników.
 *
 */

/*! \brief	Lekka interpretacja drzewa.
 *  \tparam	Data	Typ przechowywanych danych.
 *  \note		Do użycia wymagany jest konstruktor kopiujący dla danych.
 *
 * Prosta i lekka interpretacja drzewa elementów. Wspiera możliwość iteracji po zakresie (dla obecnego poziomu).
 *
 */

/*! \brief	Lekka interpretacja drzewa.
 *  \tparam	Data Typ przechowywanych danych.
 *
 * Prosta i lekka interpretacja drzewa elementów.
 *
 */
template<typename Data>
class KLTree
{

	/*! \brief		Struktura elementu drzewa.
	 *
	 * Przechowuje dane pojedynczego elementu drzewa i umożliwia usunięcie go.
	 *
	 */
	protected: struct KLTreeItem
	{

		KLTreeItem* Next;
		KLTreeItem* Branch;
		KLTreeItem* Root;

		Data* Record;

		KLTreeItem(void);

		~KLTreeItem(void);

	};

	/*! \brief		Wyliczenie specjalnych węzłów.
	 *
	 * Jednoznacznie określa specjalne indeksy dla wybranych węzłów.
	 *
	 */
	public: enum NOODES : int
	{
		ROOT = -1,	//!< Węzeł główny.
		PREV = -2,	//!< Poprzedni węzeł.
		CURRENT = -3	//!< Obecny węzeł.
	};

	public: class KLTreeVarIterator
	{

		protected:

			KLTreeItem* Current;

		public:

			KLTreeVarIterator(KLTreeItem* Begin);

			Data& operator* (void);
			KLTreeVarIterator& operator++ (void);
			bool operator!= (const KLTreeVarIterator& Iterator) const;

	};

	public: class KLTreeConstIterator
	{

		protected:

			KLTreeItem* Current;

		public:

			KLTreeConstIterator(KLTreeItem* Begin);

			const Data& operator* (void) const;
			KLTreeConstIterator& operator++ (void);
			bool operator!= (const KLTreeConstIterator& Iterator) const;

	};

	protected:

		KLTreeItem* Root;		//!< Wskaźnik na główny węzeł.

		KLTreeItem* Current;	//!< Wskaźnik na obecny węzeł.

		/*! \brief		Specjalny konstruktor.
		 *  \param [in]	Branch Gałąź główna.
		 *
		 * Tworzy tymczasowy obiekt będący ograniczeniem bazowego obiektu.
		 *
		 */
		explicit KLTree(KLTreeItem* Branch);

		/*! \brief		Usuwa gałąź.
		 *  \param [in]	Branch Gałąź do usunięcia.
		 *  \return		Ilość usuniętych pozycji w bazowym zakresie.
		 *
		 * Rekurencyjnie usuwa gałąź i jej składniki.
		 *
		 */
		int Delete(KLTreeItem* Branch);

		/*! \brief		Dodaje gałąź.
		 *  \param [in]	Branch Gałąź do wstawienia.
		 *  \return		Ilość dodanych pozycji w bazowym zakresie.
		 *
		 * Rekurencyjnie dodaje gałąź i jej składniki.
		 *
		 */
		int Insert(KLTreeItem* Branch);

	public:

		/*! \brief		Konstruktor kopiujący.
		 *  \param [in]	Tree Drzewo do skopiowania.
		 *
		 * Rekurencyjnie kopiuje całą strukturę drzewa.
		 *
		 */
		KLTree(const KLTree<Data>& Tree);

		/*! \brief		Konstruktor przenoszący.
		 *  \param [in]	Tree Drzewo do przeniesienia.
		 *
		 * Przenosi tymczasowy obiekt do bierzącego.
		 *
		 */
		KLTree(KLTree<Data>&& Tree);

		/*! \brief		Domyślny konstruktor.
		 *
		 * Inicjuje wszystkie pola obiektu.
		 *
		 */
		KLTree(void);

		/*! \brief		Destruktor.
		 *
		 * Automatycznie usuwa przechowywane dane. Nie niszczy obiektu jeśli jest on gałęzią innego.
		 *
		 */
		~KLTree(void);

		/*! \brief		Wstawianie elementu.
		 *  \param [in]	Item Element dodawany do drzewa.
		 *  \return		Aktualna liczba elementów.
		 *
		 * Dodaje do drzewa kopie podanego elementu i zwraca nową ilość elementów w bieżącym zakresie.
		 *
		 */
		int Insert(const Data& Item);

		/*! \brief		Wstawianie elementu.
		 *  \param [in]	Tree Drzewo dodawane do drzewa.
		 *  \return		Aktualna liczba elementów.
		 *
		 * Dodaje do drzewa kopie podanego drzewa i zwraca nową ilość elementów w bieżącym zakresie.
		 *
		 */
		int Insert(const KLTree<Data>& Tree);

		/*! \brief		Usunięcie elementu.
		 *  \param [in]	ID Indeks elementu numerowany od zera.
		 *  \return		Powodzenie operacji.
		 *
		 * Usuwa wybrany element i zwraca stan operacji.
		 *
		 */
		bool Delete(int ID);

		/*! \brief		Wybór gałęzi.
		 *  \param [in]	ID Indeks elementu numerowany od zera.
		 *  \return		Powodzenie operacji.
		 *  \see			KLTree::NOODES.
		 *
		 * Wybiera podaną gałąź jako roboczą.
		 *
		 */
		bool Select(int ID);

		/*! \brief		Sprawdzenie ilości elementów.
		 *  \return		Aktualna liczba elementów.
		 *
		 * Zwraca aktualną liczbę elementów w bierzącej gałęzi.
		 *
		 */
		int Size(void) const;

		/*! \brief		Sprawdzenie głębokości.
		 *  \return		Aktualna głębokość.
		 *
		 * Zwraca aktualną głębokość na jakiej znajduję się bierząca gałąź.
		 *
		 */
		int Deep(void) const;

		/*! \brief		Wybór gałęzi.
		 *  \param [in]	ID Indeks elementu numerowany od zera.
		 *  \return		Obiekt tymczasowy reprezentujący gałąź.
		 *  \see			KLTree::NOODES.
		 *
		 * Zwraca tymczasowy obiekt będący referencją do wybranej gałęzi.
		 *
		 */
		KLTree<Data> Branch(int ID);

		/*! \brief		Wybór gałęzi.
		 *  \param [in]	ID Indeks elementu numerowany od zera.
		 *  \return		Obiekt tymczasowy reprezentujący gałąź.
		 *  \see			KLTree::NOODES.
		 *
		 * Zwraca tymczasowy obiekt będący referencją do wybranej gałęzi.
		 *
		 */
		const KLTree<Data> Branch(int ID) const;

		/*! \brief		Czyszczenie drzewa.
		 *
		 * Usuwa wszystkie elementy drzewa.
		 *
		 */
		void Clean(void);

		/*! \brief		Wybór elementu.
		 *  \param [in]	ID Indeks elementu.
		 *  \return		Referencja do wybranego elementu.
		 *  \warning		Gdy element o podanym indeksie nie istnieje to zwrócona zostanie niepoprawna referencja do `nullptr` co zapewne spowoduje krytyczny wyjątek.
		 *
		 * Wybiera element o podanym indeksie z drzewa.
		 *
		 */
		Data& operator[] (int ID);

		/*! \brief		Wybór elementu.
		 *  \param [in]	ID Indeks elementu.
		 *  \return		Stała referencja do wybranego elementu.
		 *  \warning		Gdy element o podanym indeksie nie istnieje to zwrócona zostanie niepoprawna referencja do `nullptr` co zapewne spowoduje krytyczny wyjątek.
		 *
		 * Wybiera element o podanym indeksie z drzewa.
		 *
		 */
		const Data& operator[] (int ID) const;

		/*! \brief		Operator przypisania.
		 *  \param [in]	Tree Obiekt do sklonowania.
		 *  \return		Referencja do bierzącego obiektu.
		 *
		 * Zwalnia dotychczasowe zasoby i klonuje wybrany obiekt.
		 *
		 */
		KLTree<Data>& operator= (const KLTree<Data>& Tree);

		/*! \brief		Operator przeniesienia.
		 *  \param [in]	Tree Obiekt do przeniesienia.
		 *  \return		Referencja do bierzącego obiektu.
		 *
		 * Zwalnia dotychczasowe zasoby i przenosi wybrany obiekt.
		 *
		 */
		KLTree<Data>& operator= (KLTree<Data>&& Tree);

		template<typename ...Steps> void Select(int ID, Steps... IDS);

		KLTreeVarIterator begin(void);
		KLTreeVarIterator end(void);

		KLTreeConstIterator begin(void) const;
		KLTreeConstIterator end(void) const;
};

#include "kltree.cpp"

#endif // KLTREE_HPP
