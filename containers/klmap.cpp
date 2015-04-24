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

#ifndef KLMAP_CPP
#define KLMAP_CPP

#include "klmap.hpp"

template<typename Data, typename Key>
KLMap<Data, Key>::KLMapRecord::KLMapRecord(const Data& _Value, const Key& _ID)
: Value(_Value), ID(_ID) {}

template<typename Data, typename Key>
KLMap<Data, Key>::KLMapItem::~KLMapItem(void)
{
	delete Record;
}

template<typename Data, typename Key>
KLMap<Data, Key>::KLMapVarIterator::KLMapVarIterator(KLMapItem* Begin)
: Current(Begin) {}

template<typename Data, typename Key>
typename KLMap<Data, Key>::KLMapRecord& KLMap<Data, Key>::KLMapVarIterator::operator* (void)
{
	return *Current->Record;
}

template<typename Data, typename Key>
typename KLMap<Data, Key>::KLMapVarIterator& KLMap<Data, Key>::KLMapVarIterator::operator++ (void)
{
	Current = Current->Next;

	return *this;
}


template<typename Data, typename Key>
bool KLMap<Data, Key>::KLMapVarIterator::operator!= (const KLMapVarIterator& Iterator) const
{
	return Current != Iterator.Current;
}

template<typename Data, typename Key>
KLMap<Data, Key>::KLMapConstIterator::KLMapConstIterator(KLMapItem* Begin)
: Current(Begin) {}

template<typename Data, typename Key>
const typename KLMap<Data, Key>::KLMapRecord& KLMap<Data, Key>::KLMapConstIterator::operator* (void) const
{
	return *Current->Record;
}

template<typename Data, typename Key>
typename KLMap<Data, Key>::KLMapConstIterator& KLMap<Data, Key>::KLMapConstIterator::operator++ (void)
{
	Current = Current->Next;

	return *this;
}


template<typename Data, typename Key>
bool KLMap<Data, Key>::KLMapConstIterator::operator!= (const KLMapConstIterator& Iterator) const
{
	return Current != Iterator.Current;
}

template<typename Data, typename Key>
KLMap<Data, Key>::KLMap(void)
: Begin(nullptr), End(nullptr), Capacity(0) {}

template<typename Data, typename Key>
KLMap<Data, Key>::~KLMap(void)
{
	Clean();
}

template<typename Data, typename Key>
int KLMap<Data, Key>::Insert(const Data& Item, const Key& ID)
{
	if (!Begin)
	{
		Begin = End = new KLMapItem;
	}
	else
	{
		End = End->Next =new KLMapItem;
	}

	End->Record = new KLMapRecord(Item, ID);
	End->Next = nullptr;

	return ++Capacity;
}

template<typename Data, typename Key>
int KLMap<Data, Key>::Delete(const Key& ID)
{
	KLMapItem* MapItem = Begin;
	KLMapItem* PrevItem = nullptr;

	while (MapItem)
	{
		if (MapItem->Record->ID == ID)
		{
			if (!PrevItem)
				Begin = MapItem->Next;
			else
				PrevItem->Next = MapItem->Next;

			delete MapItem;

			return --Capacity;
		}
		else
		{
			PrevItem = MapItem;
			MapItem = MapItem->Next;
		}
	}

	return -1;
}

template<typename Data, typename Key>
int KLMap<Data, Key>::Size(void) const
{
	return Capacity;
}

template<typename Data, typename Key>
void KLMap<Data, Key>::Clean(void)
{
	while (Begin)
	{
		KLMapItem* MapItem = Begin->Next;

		delete Begin;

		Begin = MapItem;
	}

	Begin = nullptr;
	Capacity = 0;
}

template<typename Data, typename Key>
typename KLMap<Data, Key>::KLMapVarIterator KLMap<Data, Key>::begin(void)
{
	return KLMapVarIterator(Begin);
}

template<typename Data, typename Key>
typename KLMap<Data, Key>::KLMapVarIterator KLMap<Data, Key>::end(void)
{
	return KLMapVarIterator(nullptr);
}

template<typename Data, typename Key>
typename KLMap<Data, Key>::KLMapConstIterator KLMap<Data, Key>::begin(void) const
{
	return KLMapConstIterator(Begin);
}

template<typename Data, typename Key>
typename KLMap<Data, Key>::KLMapConstIterator KLMap<Data, Key>::end(void) const
{
	return KLMapConstIterator(nullptr);
}

template<typename Data, typename Key>
Data& KLMap<Data, Key>::operator[] (const Key& ID)
{
	KLMapItem* MapItem = Begin;

	while (MapItem)
	{
		if (MapItem->Record->ID == ID)
			return MapItem->Record->Value;
		else
			MapItem = MapItem->Next;
	}

	return *((Data*)(nullptr));
}

#endif // KLMAP_CPP
