/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Lightweight Map interpretation for KLLibs                              *
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

#ifndef KLMAP_CPP
#define KLMAP_CPP

#include "klmap.hpp"

template<typename Data, typename Key>
KLMap<Data, Key>::KLMapRecord::KLMapRecord(const Data& _Value, const Key& _ID)
: Value(_Value), ID(_ID) {}

template<typename Data, typename Key>
KLMap<Data, Key>::KLMapItem::KLMapItem(void)
: Next(nullptr), Record(nullptr) {}

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
KLMap<Data, Key>::KLMap(const KLMap<Data, Key>& Map)
: KLMap()
{
	KLMapItem* MapItem = Map.Begin;

	while (MapItem)
	{
		Insert(MapItem->Record->Value, MapItem->Record->ID);

		MapItem = MapItem->Next;
	}
}

template<typename Data, typename Key>
KLMap<Data, Key>::KLMap(KLMap<Data, Key>&& Map)
: Begin(Map.Begin), End(Map.End), Capacity(Map.Capacity)
{
	Map.Begin = nullptr;
	Map.End = nullptr;
	Map.Capacity = 0;
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
	if (Exists(ID)) return -1;

	if (!Begin)
	{
		Begin = End = new KLMapItem;
	}
	else
	{
		End = End->Next = new KLMapItem;
	}

	End->Record = new KLMapRecord(Item, ID);

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

	if (MapItem == Begin) Begin = nullptr;
	if (MapItem == End) End = PrevItem;

	return -1;
}

template<typename Data, typename Key>
bool KLMap<Data, Key>::Exists(const Key& ID) const
{
	KLMapItem* MapItem = Begin;

	while (MapItem)
	{
		if (MapItem->Record->ID == ID)
			return true;
		else
			MapItem = MapItem->Next;
	}

	return false;
}

template<typename Data, typename Key>
bool KLMap<Data, Key>::Update(const Key& OldID, const Key& NewID)
{
	KLMapItem* MapItem = Begin;

	while (MapItem)
	{
		if (MapItem->Record->ID == OldID)
		{
			MapItem->Record->ID = NewID;

			return true;
		}
		else
			MapItem = MapItem->Next;
	}

	return false;
}

template<typename Data, typename Key>
int KLMap<Data, Key>::Size(void) const
{
	return Capacity;
}

template<typename Data, typename Key>
KLList<Data> KLMap<Data, Key>::Values(void) const
{
	KLList<Data> Buffer;

	KLMapItem* MapItem = Begin;

	while (MapItem)
	{
		Buffer.Insert(MapItem->Record->Value);

		MapItem = MapItem->Next;
	}

	return Buffer;
}

template<typename Data, typename Key>
KLList<Key> KLMap<Data, Key>::Keys(void) const
{
	KLList<Key> Buffer;

	KLMapItem* MapItem = Begin;

	while (MapItem)
	{
		Buffer.Insert(MapItem->Record->ID);

		MapItem = MapItem->Next;
	}

	return Buffer;
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

	return *((Data*) nullptr);
}

template<typename Data, typename Key>
const Data& KLMap<Data, Key>::operator[] (const Key& ID) const
{
	KLMapItem* MapItem = Begin;

	while (MapItem)
	{
		if (MapItem->Record->ID == ID)
			return MapItem->Record->Value;
		else
			MapItem = MapItem->Next;
	}

	return *((Data*) nullptr);
}

template<typename Data, typename Key>
KLMap<Data, Key>& KLMap<Data, Key>::operator= (const KLMap<Data, Key>& Map)
{
	if (this == &Map) return *this;

	KLMapItem MapItem = Map.Begin;

	Clean();

	while (MapItem)
	{
		Insert(MapItem->Record->Value, MapItem->Record->ID);

		MapItem = MapItem->Next;
	}

	return *this;
}

template<typename Data, typename Key>
KLMap<Data, Key>& KLMap<Data, Key>::operator= (KLMap<Data, Key>&& Map)
{
	if (this == &Map) return *this;

	Clean();

	Begin = Map.Begin;
	End = Map.End;
	Capacity = Map.Capacity;

	Map.Begin = nullptr;
	Map.End = nullptr;
	Map.Capacity = 0;

	return *this;
}

#endif // KLMAP_CPP
