/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Variables management system interpretation for KLLibs                  *
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

#include "klvariables.hpp"

KLVariables::KLVariable::KLVariable(const KLVariable& Object)
: Pointer(Object.Pointer), Variable(Object.Variable), Callback(Object.Callback), Type(Object.Type) {}

KLVariables::KLVariable::KLVariable(TYPE VarType, void* Bind, KLSCALLBACK Handler)
: Pointer(Bind), Variable(0.0), Callback(Handler), Type(VarType) {}

KLVariables::KLVariable::KLVariable(bool Boolean, KLSCALLBACK Handler)
: KLVariable(BOOLEAN, nullptr, Handler)
{
	*this = Boolean;
}

KLVariables::KLVariable::KLVariable(double Number, KLSCALLBACK Handler)
: KLVariable(NUMBER, nullptr, Handler)
{
	*this = Number;
}

KLVariables::KLVariable::KLVariable(int Integer, KLSCALLBACK Handler)
: KLVariable(INTEGER, nullptr, Handler)
{
	*this = Integer;
}

KLString KLVariables::KLVariable::ToString(void) const
{
	if (Pointer) switch (Type)
	{
		case BOOLEAN:
			return KLString(*reinterpret_cast<bool*>(Pointer));

		case NUMBER:
			return KLString(*reinterpret_cast<double*>(Pointer));

		case INTEGER:
			return KLString(*reinterpret_cast<int*>(Pointer));
	}

	return KLString(Variable);
}

double KLVariables::KLVariable::ToNumber(void) const
{
	if (Pointer) switch (Type)
	{
		case BOOLEAN:
			return *reinterpret_cast<bool*>(Pointer);

		case NUMBER:
			return *reinterpret_cast<double*>(Pointer);

		case INTEGER:
			return *reinterpret_cast<int*>(Pointer);
	}

	return Variable;
}

int KLVariables::KLVariable::ToInt(void) const
{
	return ToNumber();
}

bool KLVariables::KLVariable::ToBool(void) const
{
	return ToNumber();
}

bool KLVariables::KLVariable::Binded(void) const
{
	return Pointer;
}

void KLVariables::KLVariable::SetCallback(KLSCALLBACK Handler)
{
	Callback = Handler;
}

KLVariables::KLSCALLBACK KLVariables::KLVariable::GetCallback(void) const
{
	return Callback;
}

template<typename Data>
KLVariables::KLVariable& KLVariables::KLVariable::operator= (const Data& Value)
{
	if (Pointer) switch (Type)
	{
		case BOOLEAN:
			*reinterpret_cast<bool*>(Pointer) = Value;
		break;
		case NUMBER:
			*reinterpret_cast<double*>(Pointer) = Value;
		break;
		case INTEGER:
			*reinterpret_cast<int*>(Pointer) = Value;
		break;
	}
	else Variable = Value;

	if (Callback) Callback(ToNumber());

	return *this;
}

KLVariables::KLVariables(KLVariables* Scoope)
: Parent(Scoope) {}

KLVariables::KLVariables(const KLVariables& Objects)
: Variables(Objects.Variables), Parent(Objects.Parent) {}

bool KLVariables::Add(const KLString& Name, const KLVariable& Object)
{
	return Variables.Insert(Object, Name) != -1;
}

bool KLVariables::Add(const KLString& Name, TYPE Type, KLSCALLBACK Handler)
{
	return Variables.Insert(KLVariable(Type, nullptr, Handler), Name) != -1;
}

bool KLVariables::Add(const KLString& Name, bool& Boolean, KLSCALLBACK Handler)
{
	return Variables.Insert(KLVariable(BOOLEAN, &Boolean, Handler), Name) != -1;
}

bool KLVariables::Add(const KLString& Name, double& Number, KLSCALLBACK Handler)
{
	return Variables.Insert(KLVariable(NUMBER, &Number, Handler), Name) != -1;
}

bool KLVariables::Add(const KLString& Name, int& Integer, KLSCALLBACK Handler)
{
	return Variables.Insert(KLVariable(INTEGER, &Integer, Handler), Name) != -1;
}

bool KLVariables::Delete(const KLString& Name)
{
	return Variables.Delete(Name) != -1;
}

bool KLVariables::Exists(const KLString& Name, bool Recursive) const
{
	if (Parent && Recursive)
		return Variables.Exists(Name) || Parent->Exists(Name);
	else
		return Variables.Exists(Name);
}

int KLVariables::Size(void) const
{
	return Variables.Size();
}

void KLVariables::Clean(void)
{
	Variables.Clean();
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

KLVariables& KLVariables::operator = (const KLVariables& Objects)
{
	if (&Objects == this) return *this;

	Clean();

	for (const auto& Var: Objects) Add(Var.ID, Var.Value);

	return *this;
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

template KLVariables::KLVariable& KLVariables::KLVariable::operator= (const double&);
template KLVariables::KLVariable& KLVariables::KLVariable::operator= (const int&);
template KLVariables::KLVariable& KLVariables::KLVariable::operator= (const bool&);
