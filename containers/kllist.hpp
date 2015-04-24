/***********************************************************************
 *
 * {description}
 * Copyright (C) {year}  {fullname}
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

#ifndef KLLIST_HPP
#define KLLIST_HPP

template<typename Data>
class KLList
{

	protected: struct KLListItem
	{

		KLListItem* Next;
		Data* Record;

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

		KLListItem* Begin;
		KLListItem* End;

		int Capacity;

	public:

		KLList(void);
		~KLList(void);

		int Insert(const Data& Item);
		int Delete(int ID);

		int Size(void) const;

		void Clean(void);

		KLListVarIterator begin(void);
		KLListVarIterator end(void);

		KLListConstIterator begin(void) const;
		KLListConstIterator end(void) const;

		Data& operator[] (int ID);

};

#include "kllist.cpp"

#endif // KLLIST_HPP
