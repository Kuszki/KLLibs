/***********************************************************************
 *
 *  Lightweight List interpretation for KLLibs
 *  Copyright (C) 2015  Łukasz "Kuszki" Dróżdż
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses/.
 *
 **********************************************************************/

#ifndef KLLIST_CPP
#define KLLIST_CPP

#include "kllist.hpp"

template<typename Data>
KLList<Data>::KLListItem::KLListItem(void)
: Next(nullptr), Record(nullptr) {}

template<typename Data>
KLList<Data>::KLListItem::~KLListItem(void)
{
	delete Record;
}

template<typename Data>
KLList<Data>::KLListVarIterator::KLListVarIterator(KLListItem* Begin)
: Current(Begin) {}

template<typename Data>
Data& KLList<Data>::KLListVarIterator::operator* (void)
{
	return *Current->Record;
}

template<typename Data>
typename KLList<Data>::KLListVarIterator& KLList<Data>::KLListVarIterator::operator++ (void)
{
	Current = Current->Next;

	return *this;
}


template<typename Data>
bool KLList<Data>::KLListVarIterator::operator!= (const KLListVarIterator& Iterator) const
{
	return Current != Iterator.Current;
}

template<typename Data>
KLList<Data>::KLListConstIterator::KLListConstIterator(KLListItem* Begin)
: Current(Begin) {}

template<typename Data>
const Data& KLList<Data>::KLListConstIterator::operator* (void) const
{
	return *Current->Record;
}

template<typename Data>
typename KLList<Data>::KLListConstIterator& KLList<Data>::KLListConstIterator::operator++ (void)
{
	Current = Current->Next;

	return *this;
}


template<typename Data>
bool KLList<Data>::KLListConstIterator::operator!= (const KLListConstIterator& Iterator) const
{
	return Current != Iterator.Current;
}

template<typename Data>
KLList<Data>::KLList(const KLList<Data>& List)
: KLList()
{
	KLListItem* ListItem = List.Begin;

	while (ListItem)
	{
		Insert(*ListItem->Record);

		ListItem = ListItem->Next;
	}
}

template<typename Data>
KLList<Data>::KLList(KLList<Data>&& List)
: Begin(List.Begin), End(List.End), Capacity(List.Capacity)
{
	List.Begin = nullptr;
	List.End = nullptr;
	List.Capacity = 0;
}

template<typename Data>
KLList<Data>::KLList(void)
: Begin(nullptr), End(nullptr), Capacity(0) {}

template<typename Data>
KLList<Data>::~KLList(void)
{
	Clean();
}

template<typename Data>
int KLList<Data>::Insert(const Data& Item)
{
	if (!Begin)
	{
		Begin = End = new KLListItem;
	}
	else
	{
		End = End->Next = new KLListItem;
	}

	End->Record = new Data(Item);

	return ++Capacity;
}

template<typename Data>
int KLList<Data>::Delete(int ID)
{
	KLListItem* ListItem = Begin;
	KLListItem* PrevItem = nullptr;

	for (int i = 0; i < ID; i++)
	{
		if (ListItem)
		{
			PrevItem = ListItem;
			ListItem = ListItem->Next;
		}
		else
			return -1;
	}

	if (!PrevItem)
		Begin = ListItem->Next;
	else
		PrevItem->Next = ListItem->Next;

	if (ListItem == Begin) Begin = nullptr;
	if (ListItem == End) End = PrevItem;

	delete ListItem;

	return --Capacity;
}

template<typename Data>
Data KLList<Data>::Dequeue(void)
{
	if (!Begin) return Data();

	Data Buffer(*Begin->Record);

	Delete(0);

	return Buffer;
}

template<typename Data>
Data KLList<Data>::Pop(void)
{
	if (!End) return Data();

	Data Buffer(*End->Record);

	Delete(Capacity - 1);

	return Buffer;
}

template<typename Data>
Data& KLList<Data>::First(void)
{
	return *Begin->Record;
}

template<typename Data>
const Data& KLList<Data>::First(void) const
{
	return *Begin->Record;
}

template<typename Data>
Data& KLList<Data>::Last(void)
{
	return *End->Record;
}

template<typename Data>
const Data& KLList<Data>::Last(void) const
{
	return *End->Record;
}

template<typename Data>
int KLList<Data>::Size(void) const
{
	return Capacity;
}

template<typename Data>
void KLList<Data>::Clean(void)
{
	while (Begin)
	{
		KLListItem* ListItem = Begin->Next;

		delete Begin;

		Begin = ListItem;
	}

	Begin = End = nullptr;
	Capacity = 0;
}

template<typename Data>
typename KLList<Data>::KLListVarIterator KLList<Data>::begin(void)
{
	return KLListVarIterator(Begin);
}

template<typename Data>
typename KLList<Data>::KLListVarIterator KLList<Data>::end(void)
{
	return KLListVarIterator(nullptr);
}

template<typename Data>
typename KLList<Data>::KLListConstIterator KLList<Data>::begin(void) const
{
	return KLListConstIterator(Begin);
}

template<typename Data>
typename KLList<Data>::KLListConstIterator KLList<Data>::end(void) const
{
	return KLListConstIterator(nullptr);
}

template<typename Data>
Data& KLList<Data>::operator[] (int ID)
{
	switch (ID)
	{
		case FIRST:
			return *Begin->Record;
		break;
		case LAST:
			return *End->Record;
		break;
		default:
			KLListItem* ListItem = Begin;

			for (int i = 0; i < ID; i++)
			{
				if (ListItem)
					ListItem = ListItem->Next;
				else
					return *((Data*) nullptr);
			}

			return *ListItem->Record;
	}
}

template<typename Data>
const Data& KLList<Data>::operator[] (int ID) const
{
	switch (ID)
	{
		case FIRST:
			return *Begin->Record;
		break;
		case LAST:
			return *End->Record;
		break;
		default:
			KLListItem* ListItem = Begin;

			for (int i = 0; i < ID; i++)
			{
				if (ListItem)
					ListItem = ListItem->Next;
				else
					return *((Data*) nullptr);
			}

			return *ListItem->Record;
	}
}

template<typename Data>
KLList<Data>& KLList<Data>::operator<< (const Data& Item)
{
	Insert(Item);

	return *this;
}

template<typename Data>
KLList<Data>& KLList<Data>::operator= (const KLList<Data>& List)
{
	if (this == &List) return *this;

	KLListItem* ListItem = List.Begin;

	Clean();

	while (ListItem)
	{
		Insert(*ListItem->Record);

		ListItem = ListItem->Next;
	}

	return *this;
}

#endif // KLLIST_CPP
