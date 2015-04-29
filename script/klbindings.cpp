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

bool KLBindings::Add(const KLString& Name, KLSENTRY Entry)
{
	if (!Entry) return false;

	return Bindings.Insert(Entry, Name);
}

bool KLBindings::Delete(const KLString& Name)
{
	return Bindings.Delete(Name) != -1;
}

bool KLBindings::Call(const KLString& Name, KLVariables& Scoope)
{
	if (!Bindings.Exists(Name)) return false;

	Bindings[Name](Scoope);

	return true;
}

bool KLBindings::Update(const KLString& Name, KLSENTRY Entry)
{
	if (!Bindings.Exists(Name)) return false;

	Bindings[Name] = Entry;

	return true;
}

bool KLBindings::Exists(const KLString& Name) const
{
	return Bindings.Exists(Name);
}
