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

#include "klvariables.hpp"

KLVariables::KLVariable::KLVariable(const KLVariable& Object)
: Binded(Object.Binded), Type(Object.Type)
{
	if (!Binded)
		switch (Type)
		{
			case STRING:
				Pointer = new KLString(*((KLString*) Object.Pointer));
			break;
			case NUMBER:
				Pointer = new double(*((double*) Object.Pointer));
			break;
			case INTEGER:
				Pointer = new int(*((int*) Object.Pointer));
			break;
		}
	else
		Pointer = Object.Pointer;
}

KLVariables::KLVariable::KLVariable(TYPE VarType, void* Bind)
: Binded(Bind), Type(VarType)
{
	if (!Binded)
		switch (Type)
		{
			case STRING:
				Pointer = new KLString();
			break;
			case NUMBER:
				Pointer = new double;
			break;
			case INTEGER:
				Pointer = new int;
			break;
		}
	else
		Pointer = Bind;
}

KLVariables::KLVariable::~KLVariable(void)
{
	if (!Binded)
		switch (Type)
		{
			case STRING:
				delete ((KLString*) Pointer);
			break;
			default:
				delete (char*) Pointer;
		}
}

KLString KLVariables::KLVariable::ToString(void) const
{
	switch (Type)
	{
		case STRING:
			return KLString(*((KLString*) Pointer));
		break;
		case NUMBER:
			return KLString(*((double*) Pointer));
		break;
		case INTEGER:
			return KLString(*((int*) Pointer));
		break;
	}

	return KLString();
}

double KLVariables::KLVariable::ToNumber(void) const
{
	switch (Type)
	{
		case STRING:
			return ((KLString*) Pointer)->ToNumber();
		break;
		case NUMBER:
			return *((double*) Pointer);
		break;
		case INTEGER:
			return *((int*) Pointer);
		break;
	}

	return double();
}

int KLVariables::KLVariable::ToInt(void) const
{
	switch (Type)
	{
		case STRING:
			return ((KLString*) Pointer)->ToInt();
		break;
		case NUMBER:
			return *((double*) Pointer);
		break;
		case INTEGER:
			return *((int*) Pointer);
		break;
	}

	return int();
}

void KLVariables::KLVariable::Set(const KLString& String)
{
	switch (Type)
	{
		case STRING:
			*((KLString*) Pointer) = String;
		break;
		case NUMBER:
			*((double*) Pointer) = String.ToNumber();
		break;
		case INTEGER:
			*((int*) Pointer) = String.ToInt();
		break;
	}
}

void KLVariables::KLVariable::Set(double Number)
{
	switch (Type)
	{
		case STRING:
			*((KLString*) Pointer) = KLString(Number);
		break;
		case NUMBER:
			*((double*) Pointer) = Number;
		break;
		case INTEGER:
			*((int*) Pointer) = Number;
		break;
	}
}

void KLVariables::KLVariable::Set(int Integer)
{
	switch (Type)
	{
		case STRING:
			*((KLString*) Pointer) = KLString(Integer);
		break;
		case NUMBER:
			*((double*) Pointer) = Integer;
		break;
		case INTEGER:
			*((int*) Pointer) = Integer;
		break;
	}
}

KLVariables::KLVariable& KLVariables::KLVariable::operator= (const KLVariable& Object)
{
	if (this == &Object) return *this;

	switch (Type)
	{
		case STRING:
			*((KLString*) Pointer) = Object.ToString();
		break;
		case NUMBER:
			*((double*) Pointer) = Object.ToNumber();
		break;
		case INTEGER:
			*((int*) Pointer) = Object.ToInt();
		break;
	}

	return *this;
}

KLVariables::KLVariables(KLVariables* Parent)
: UpperScoope(Parent) {}

KLVariables::KLVariables(const KLVariables& Objects)
: Variables(Objects.Variables), UpperScoope(Objects.UpperScoope) {}

bool KLVariables::Add(const KLString& Name, TYPE Type)
{
	if (Variables.Exists(Name))
		return false;
	else
		return Variables.Insert(KLVariable(Type), Name);
}

bool KLVariables::Add(const KLString& Name, KLString& String)
{
	if (Variables.Exists(Name))
		return false;
	else
		return Variables.Insert(KLVariable(STRING, &String), Name);
}

bool KLVariables::Add(const KLString& Name, double& Number)
{
	if (Variables.Exists(Name))
		return false;
	else
		return Variables.Insert(KLVariable(NUMBER, &Number), Name);
}

bool KLVariables::Add(const KLString& Name, int& Integer)
{
	if (Variables.Exists(Name))
		return false;
	else
		return Variables.Insert(KLVariable(INTEGER, &Integer), Name);
}

bool KLVariables::Delete(const KLString& Name)
{
	return Variables.Delete(Name) != -1;
}

bool KLVariables::Exists(const KLString& Name) const
{
	if (UpperScoope)
		return Variables.Exists(Name) || UpperScoope->Exists(Name);
	else
		return Variables.Exists(Name);
}

KLVariables::KLVariable& KLVariables::operator[] (const KLString& Name)
{
	if (!Variables.Exists(Name))
		return (*UpperScoope)[Name];
	else
		return Variables[Name];
}

const KLVariables::KLVariable& KLVariables::operator[] (const KLString& Name) const
{
	if (!Variables.Exists(Name))
		return (*UpperScoope)[Name];
	else
		return Variables[Name];
}

KLMap<KLVariables::KLVariable, KLString>::KLMapVarIterator KLVariables::begin(void)
{
	return Variables.begin();
}

KLMap<KLVariables::KLVariable, KLString>::KLMapVarIterator KLVariables::end(void)
{
	return Variables.end();
}

KLMap<KLVariables::KLVariable, KLString>::KLMapConstIterator KLVariables::begin(void) const
{
	return Variables.begin();
}

KLMap<KLVariables::KLVariable, KLString>::KLMapConstIterator KLVariables::end(void) const
{
	return Variables.end();
}
