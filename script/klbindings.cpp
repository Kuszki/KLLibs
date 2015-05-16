/***********************************************************************
 *
 * Function binding management system interpretation for KLLibs
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

#include "klbindings.hpp"

KLBindings::KLBinding::KLBinding(const KLBinding& Binding)
: Pointer(Binding.Pointer) {}

KLBindings::KLBinding::KLBinding(KLSENTRY Entry)
: Pointer(Entry) {}

void KLBindings::KLBinding::Update(KLSENTRY Entry)
{
	Pointer = Entry;
}

double KLBindings::KLBinding::operator() (KLVariables& Variables)
{
	return Pointer(Variables);
}

bool KLBindings::Add(const KLString& Name, KLSENTRY Entry)
{
	if (!Entry) return false;

	return Bindings.Insert(Entry, Name);
}

bool KLBindings::Delete(const KLString& Name)
{
	return Bindings.Delete(Name) != -1;
}

bool KLBindings::Exists(const KLString& Name) const
{
	return Bindings.Exists(Name);
}

int KLBindings::Size(void) const
{
	return Bindings.Size();
}

KLBindings::KLBinding& KLBindings::operator[] (const KLString& Name)
{
	return Bindings[Name];
}

const KLBindings::KLBinding& KLBindings::operator[] (const KLString& Name) const
{
	return Bindings[Name];
}

KLMap<KLBindings::KLBinding, KLString>::KLMapVarIterator KLBindings::begin(void)
{
	return Bindings.begin();
}

KLMap<KLBindings::KLBinding, KLString>::KLMapVarIterator KLBindings::end(void)
{
	return Bindings.end();
}

KLMap<KLBindings::KLBinding, KLString>::KLMapConstIterator KLBindings::begin(void) const
{
	return Bindings.begin();
}

KLMap<KLBindings::KLBinding, KLString>::KLMapConstIterator KLBindings::end(void) const
{
	return Bindings.end();
}
