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
			case BOOLEAN:
				Pointer = malloc(sizeof(bool));
				*((bool*) Pointer) = *((bool*) Object.Pointer);
			break;
			case NUMBER:
				Pointer = malloc(sizeof(double));
				*((double*) Pointer) = *((double*) Object.Pointer);
			break;
			case INTEGER:
				Pointer = malloc(sizeof(int));
				*((int*) Pointer) = *((int*) Object.Pointer);
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
			case BOOLEAN:
				Pointer = malloc(sizeof(bool));
			break;
			case NUMBER:
				Pointer = malloc(sizeof(double));
			break;
			case INTEGER:
				Pointer = malloc(sizeof(int));
			break;
		}
	else
		Pointer = Bind;
}

KLVariables::KLVariable::~KLVariable(void)
{
	if (!Binded) free(Pointer);
}

KLString KLVariables::KLVariable::ToString(void) const
{
	switch (Type)
	{
		case BOOLEAN:
			return KLString(*((bool*) Pointer));
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
		case BOOLEAN:
			return *((bool*) Pointer);
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
		case BOOLEAN:
			return *((bool*) Pointer);
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

int KLVariables::KLVariable::ToBool(void) const
{
	switch (Type)
	{
		case BOOLEAN:
			return *((bool*) Pointer);
		break;
		case NUMBER:
			return *((double*) Pointer);
		break;
		case INTEGER:
			return *((int*) Pointer);
		break;
	}

	return bool();
}

void KLVariables::KLVariable::Set(const KLString& String)
{
	switch (Type)
	{
		case BOOLEAN:
			*((bool*) Pointer) = String.ToBool();
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
		case BOOLEAN:
			*((bool*) Pointer) = Number;
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
		case BOOLEAN:
			*((bool*) Pointer) = Integer;
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
		case BOOLEAN:
			*((bool*) Pointer) = Object.ToBool();
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
