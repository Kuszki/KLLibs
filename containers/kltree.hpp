/***********************************************************************
 *
 * Lightweight Tree interpretation for KLLibs
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

#ifndef KLTREE_HPP
#define KLTREE_HPP

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

template<typename Data>
class KLTree
{

	protected: struct KLTreeRecord
	{

		KLTreeRecord* Next;
		KLTreeRecord* Branch;
		KLTreeRecord* Root;

		Data* Record;

		~KLTreeRecord(void);

	};

	public: enum SELECT_CODE : int
	{
		SELECT_ROOT = -1,
		SELECT_PREV = -2
	};

	protected:

		KLTreeRecord* Root;

		KLTreeRecord* Current;

		explicit KLTree(KLTreeRecord* Branch);

	public:

		KLTree(const KLTree& Tree) = delete;

		KLTree(KLTree&& Tree);

		KLTree(void);

		~KLTree(void);

		int Insert(const Data &Item,
				 int ID);

		int Insert(const KLTree& Tree,
				 int ID);

		int Delete(int ID);

		int Size(void) const;

		int Deep(void) const;

		bool Select(int ID);

		KLTree Branch(int ID);

		const KLTree Branch(int ID) const;

		Data& operator[] (int ID);

		const Data& operator[] (int ID) const;

		template<typename ...Steps> int Select(int ID, Steps... IDS);
};

#endif // KLTREE_HPP
