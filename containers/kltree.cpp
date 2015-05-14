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

#ifndef KLTREE_CPP
#define KLTREE_CPP

#include "kltree.hpp"

template<typename Data>
KLTree<Data>::KLTreeItem::KLTreeItem(void)
: Next(nullptr), Branch(nullptr), Root(nullptr), Record(nullptr) {}

template<typename Data>
KLTree<Data>::KLTreeItem::~KLTreeItem(void)
{
	delete Record;
}

template<typename Data>
KLTree<Data>::KLTreeVarIterator::KLTreeVarIterator(KLTreeItem* Begin)
: Current(Begin) {}

template<typename Data>
Data& KLTree<Data>::KLTreeVarIterator::operator* (void)
{
	return *Current->Record;
}

template<typename Data>
typename KLTree<Data>::KLTreeVarIterator& KLTree<Data>::KLTreeVarIterator::operator++ (void)
{
	Current = Current->Next;

	return *this;
}


template<typename Data>
bool KLTree<Data>::KLTreeVarIterator::operator!= (const KLTreeVarIterator& Iterator) const
{
	return Current != Iterator.Current;
}

template<typename Data>
KLTree<Data>::KLTreeConstIterator::KLTreeConstIterator(KLTreeItem* Begin)
: Current(Begin) {}

template<typename Data>
const Data& KLTree<Data>::KLTreeConstIterator::operator* (void) const
{
	return *Current->Record;
}

template<typename Data>
typename KLTree<Data>::KLTreeConstIterator& KLTree<Data>::KLTreeConstIterator::operator++ (void)
{
	Current = Current->Next;

	return *this;
}


template<typename Data>
bool KLTree<Data>::KLTreeConstIterator::operator!= (const KLTreeConstIterator& Iterator) const
{
	return Current != Iterator.Current;
}

template<typename Data>
KLTree<Data>::KLTree(KLTreeItem* Branch)
: Root(Branch), Current(Branch)
{
	Root->Root = Branch;
}

template<typename Data>
KLTree<Data>::KLTree(const KLTree<Data>& Tree)
: KLTree()
{
	Insert(Tree.Root);
}

template<typename Data>
KLTree<Data>::KLTree(KLTree<Data>&& Tree)
: Root(Tree.Root), Current(Tree.Current)
{
	Tree.Root = nullptr;
	Tree.Current = nullptr;
}

template<typename Data>
KLTree<Data>::KLTree(void)
{
	Root = Current = new KLTreeItem;
}

template<typename Data>
KLTree<Data>::~KLTree(void)
{
	if (!Root->Root)
	{
		Clean();

		delete Root;
	}
}

template<typename Data>
int KLTree<Data>::Delete(KLTreeItem* Branch)
{
	KLTreeItem* TreeItem = nullptr;

	int Count = 0;

	while (Branch)
	{
		if (Branch->Branch) Delete(Branch->Branch);

		TreeItem = Branch;
		Branch = Branch->Next;

		delete TreeItem;

		Count++;
	}

	return Count;
}

template<typename Data>
int KLTree<Data>::Insert(KLTreeItem* Branch)
{
	int Count = 0;

	while (Branch)
	{
		Insert(Branch->Record);

		if (Branch->Branch)
		{
			Select(Count);

			Insert(Branch->Branch);

			Select(PREV);
		}

		Branch = Branch->Next;

		Count++;
	}

	return Count;
}

template<typename Data>
int KLTree<Data>::Insert(const Data& Item)
{
	KLTreeItem* TreeItem;

	int Count = 0;

	if (!Current->Branch)
	{
		TreeItem = Current->Branch = new KLTreeItem;
	}
	else
	{
		TreeItem = Current->Branch;

		while (TreeItem->Next)
		{
			TreeItem = TreeItem->Next;

			Count++;
		}

		TreeItem = TreeItem->Next = new KLTreeItem;
	}

	TreeItem->Record = new Data(Item);
	TreeItem->Root = Current;

	return Count + 1;
}

template<typename Data>
int KLTree<Data>::Insert(const KLTree<Data>& Tree)
{
	KLTreeItem* TreeItem = Tree.Root;
	KLTreeItem* LastCurrent = Current;

	int Count = Insert(Tree.Root);

	Current = LastCurrent;

	return Count;
}

template<typename Data>
bool KLTree<Data>::Delete(int ID)
{
	KLTreeItem* TreeItem = Current->Branch;
	KLTreeItem* PrevItem = nullptr;

	for (int i = 0; i < ID; i++)
	{
		if (TreeItem)
		{
			PrevItem = TreeItem;
			TreeItem = TreeItem->Next;
		}
		else
			return false;
	}

	if (!PrevItem)
	{
		if (Root->Branch == Current->Branch)
			Root->Branch = Current->Branch = TreeItem->Next;
		else
			Current->Branch = TreeItem->Next;
	}
	else
		PrevItem->Next = TreeItem->Next;

	if (TreeItem->Branch) Delete(TreeItem->Branch);

	delete TreeItem;

	return true;
}

template<typename Data>
bool KLTree<Data>::Select(int ID)
{
	switch (ID)
	{
		case ROOT:
				Current = Root;
		break;
		case PREV:
			if (Current != Root)
				Current = Current->Root;
			else
				return false;
		break;
		default:
			KLTreeItem* TreeItem = Current->Branch;

			for (int i = 0; i < ID; i++)
			{
				if (TreeItem)
					TreeItem = TreeItem->Next;
				else
					return false;
			}

			Current = TreeItem;
	}

	return true;
}

template<typename Data>
int KLTree<Data>::Size(void) const
{
	KLTreeItem* TreeItem = Current->Branch;

	int Count = 0;

	while (TreeItem)
	{
		TreeItem = TreeItem->Next;

		Count++;
	}

	return Count;
}

template<typename Data>
int KLTree<Data>::Deep(void) const
{
	KLTreeItem* TreeItem = Current->Branch;

	int Count = 0;

	while (TreeItem != Root)
	{
		TreeItem = TreeItem->Root;

		Count++;
	}

	return Count;
}

template<typename Data>
KLTree<Data> KLTree<Data>::Branch(int ID)
{
	switch (ID)
	{
		case ROOT:
			return KLTree(Root);
		break;
		case PREV:
			if (Current != Root) return KLTree(Current->Root);
		break;
		case CURRENT:
			return KLTree(Current);
		default:
			KLTreeItem* TreeItem = Current->Branch;

			for (int i = 0; i < ID; i++)
			{
				if (TreeItem)
					TreeItem = TreeItem->Next;
				else
					return KLTree();
			}

			return KLTree(TreeItem);
	}

	return KLTree();
}

template<typename Data>
const KLTree<Data> KLTree<Data>::Branch(int ID) const
{
	switch (ID)
	{
		case ROOT:
			return KLTree(Root);
		break;
		case PREV:
			if (Current != Root) return KLTree(Current->Root);
		break;
		case CURRENT:
			return KLTree(Current);
		default:
			KLTreeItem* TreeItem = Current->Branch;

			for (int i = 0; i < ID; i++)
			{
				if (TreeItem)
					TreeItem = TreeItem->Next;
				else
					return KLTree();
			}

			return KLTree(TreeItem);
	}

	return KLTree();
}

template<typename Data>
void KLTree<Data>::Clean(void)
{
	if (Root->Branch)
	{
		Delete(Root->Branch);

		Current = Root;
	}
}

template<typename Data>
typename KLTree<Data>::KLTreeVarIterator KLTree<Data>::begin(void)
{
	return KLTreeVarIterator(Current->Branch);
}

template<typename Data>
typename KLTree<Data>::KLTreeVarIterator KLTree<Data>::end(void)
{
	return KLTreeVarIterator(nullptr);
}

template<typename Data>
typename KLTree<Data>::KLTreeConstIterator KLTree<Data>::begin(void) const
{
	return KLTreeConstIterator(Current->Branch);
}

template<typename Data>
typename KLTree<Data>::KLTreeConstIterator KLTree<Data>::end(void) const
{
	return KLTreeConstIterator(nullptr);
}


template<typename Data>
Data& KLTree<Data>::operator[] (int ID)
{
	KLTreeItem* TreeItem = Current->Branch;

	for (int i = 0; i < ID; i++)
	{
		if (TreeItem)
			TreeItem = TreeItem->Next;
		else
			return *((Data*) nullptr);
	}

	return *TreeItem->Record;
}

template<typename Data>
const Data& KLTree<Data>::operator[] (int ID) const
{
	KLTreeItem* TreeItem = Current->Branch;

	for (int i = 0; i < ID; i++)
	{
		if (TreeItem)
			TreeItem = TreeItem->Next;
		else
			return *((Data*) nullptr);
	}

	return *TreeItem->Record;
}

template<typename Data>
KLTree<Data>& KLTree<Data>::operator= (const KLTree<Data>& Tree)
{
	if (this == &Tree) return *this;

	Clean();

	Insert(Tree.Root);

	return *this;
}

template<typename Data>
template<typename ...Steps>
void KLTree<Data>::Select(int ID, Steps... IDS)
{
	Select(ID);

	Select(IDS...);
}

#endif // KLTREE_CPP
