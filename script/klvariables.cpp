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
: Pointer(Object.Pointer), Variable(Object.Variable), Type(Object.Type) {}

KLVariables::KLVariable::KLVariable(TYPE VarType, void* Bind)
: Pointer(Bind), Type(VarType){}

KLVariables::KLVariable::KLVariable(bool Boolean)
: KLVariable(BOOLEAN)
{
	*this = Boolean;
}

KLVariables::KLVariable::KLVariable(double Number)
: KLVariable(NUMBER)
{
	*this = Number;
}

KLVariables::KLVariable::KLVariable(int Integer)
: KLVariable(INTEGER)
{
	*this = Integer;
}

KLString KLVariables::KLVariable::ToString(void) const
{
	if (Pointer) switch (Type)
	{
		case BOOLEAN:
			return KLString(*((bool*) Pointer));
		case NUMBER:
			return KLString(*((double*) Pointer));
		case INTEGER:
			return KLString(*((int*) Pointer));
	}

	return KLString(Variable);
}

double KLVariables::KLVariable::ToNumber(void) const
{
	if (Pointer) switch (Type)
	{
		case BOOLEAN:
			return *((bool*) Pointer);
		case NUMBER:
			return *((double*) Pointer);
		case INTEGER:
			return *((int*) Pointer);
	}

	return Variable;
}

int KLVariables::KLVariable::ToInt(void) const
{
	return ToNumber();
}

int KLVariables::KLVariable::ToBool(void) const
{
	return ToNumber();
}

bool KLVariables::KLVariable::Binded(void) const
{
	return Pointer;
}

template<typename Data>
KLVariables::KLVariable& KLVariables::KLVariable::operator= (const Data& Value)
{
	if (Pointer) switch (Type)
	{
		case BOOLEAN:
			*((bool*) Pointer) = Value;
		break;
		case NUMBER:
			*((double*) Pointer) = Value;
		break;
		case INTEGER:
			*((int*) Pointer) = Value;
		break;
	}
	else Variable = Value;

	return *this;
}

KLVariables::KLVariables(KLVariables* UpperScoope)
: Parent(UpperScoope) {}

KLVariables::KLVariables(const KLVariables& Objects)
: Variables(Objects.Variables), Parent(Objects.Parent) {}

bool KLVariables::Add(const KLString& Name, const KLVariable& Object)
{
	return Variables.Insert(Object, Name) != -1;
}

bool KLVariables::Add(const KLString& Name, TYPE Type)
{
	return Variables.Insert(KLVariable(Type), Name) != -1;
}

bool KLVariables::Add(const KLString& Name, bool& Boolean)
{
	return Variables.Insert(KLVariable(BOOLEAN, &Boolean), Name) != -1;
}

bool KLVariables::Add(const KLString& Name, double& Number)
{
	return Variables.Insert(KLVariable(NUMBER, &Number), Name) != -1;
}

bool KLVariables::Add(const KLString& Name, int& Integer)
{
	return Variables.Insert(KLVariable(INTEGER, &Integer), Name) != -1;
}

bool KLVariables::Delete(const KLString& Name)
{
	return Variables.Delete(Name) != -1;
}

bool KLVariables::Exists(const KLString& Name) const
{
	if (Parent)
		return Variables.Exists(Name) || Parent->Exists(Name);
	else
		return Variables.Exists(Name);
}

int KLVariables::Size(void) const
{
	return Variables.Size();
}

KLVariables::KLVariable& KLVariables::operator[] (const KLString& Name)
{
	if (!Variables.Exists(Name))
		return (*Parent)[Name];
	else
		return Variables[Name];
}

const KLVariables::KLVariable& KLVariables::operator[] (const KLString& Name) const
{
	if (!Variables.Exists(Name))
		return (*Parent)[Name];
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
