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

#ifndef KLMAP_HPP
#define KLMAP_HPP

template<typename Data, typename Key>
class KLMap
{

	public: struct KLMapRecord
	{

		Data Value;
		Key ID;

		KLMapRecord(const Data& _Value, const Key& _ID);

	};

	protected: struct KLMapItem
	{

		KLMapItem* Next;
		KLMapRecord* Record;

		~KLMapItem(void);

	};

	protected: class KLMapVarIterator
	{

		protected:

			KLMapItem* Current;

		public:

			KLMapVarIterator(KLMapItem* Begin);

			KLMapRecord& operator* (void);
			KLMapVarIterator& operator++ (void);
			bool operator!= (const KLMapVarIterator& Iterator) const;

	};

	protected: class KLMapConstIterator
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

		KLMapItem* Begin;
		KLMapItem* End;

		int Capacity;

	public:

		KLMap(void);
		~KLMap(void);

		int Insert(const Data& Item, const Key& ID);
		int Delete(const Key& ID);

		int Size(void) const;

		void Clean(void);

		KLMapVarIterator begin(void);
		KLMapVarIterator end(void);

		KLMapConstIterator begin(void) const;
		KLMapConstIterator end(void) const;

		Data& operator[] (const Key& ID);

};

#include "klmap.cpp"

#endif // KLMAP_HPP
