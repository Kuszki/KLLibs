/***********************************************************************
 *
 * Lightweight String interpretation for KLLibs
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

#include "klstring.hpp"

template<typename Mixed>
KLString::KLString(const Mixed& Value, const char Expr[])
{
	char Buffer[32];

	snprintf(Buffer, 32, Expr, Value);

	Capacity = strlen(Buffer);

	Data = new char[Capacity + 1];

	memcpy(Data, Buffer, Capacity + 1);
}

KLString::KLString(char Char)
: Capacity(1)
{
	Data = new char[2];

	Data[0] = Char;
	Data[1] = 0;
}

KLString::KLString(const char String[])
{
	Capacity = strlen(String);

	Data = new char[Capacity + 1];

	memcpy(Data, String, Capacity + 1);
}

KLString::KLString(const KLString& String)
: KLString(String.Data) {}

KLString::KLString(void)
: Data(nullptr), Capacity(0) {}

KLString::~KLString(void)
{
	if (Data) delete [] Data;
}

int KLString::Insert(const KLString& String, int Position)
{
	char* Buffer = new char[Capacity + String.Capacity + 1];

	if (!Data)
	{
		memcpy(Buffer, String.Data, String.Capacity + 1);
	}
	else	if (Position == -1 || Position == Capacity)
	{
		memcpy(Buffer, Data, Capacity);
		memcpy(Buffer + Capacity, String.Data, String.Capacity + 1);
	}
	else
	{
		memcpy(Buffer, Data, Position);
		memcpy(Buffer + Position, String.Data, String.Capacity);
		memcpy(Buffer + Position + String.Capacity, Data + Position, Capacity - Position + 1);
	}

	if (Data) delete [] Data;

	Data = Buffer;

	return Capacity += String.Capacity;
}

int KLString::Delete(const KLString& String, bool All)
{
	if (Capacity < String.Capacity) return 0;

	int Counter = 0;
	int Found = -1;

	while ((Found = Find(String)) != -1)
	{
		memcpy(Data + Found, Data + String.Capacity, Capacity - Found + 1);

		Capacity -= String.Capacity;

		Counter++;

		if (!All) break;
	}

	char* Buffer = new char[Capacity + 1];

	memcpy(Buffer, Data, Capacity + 1);

	delete [] Data;

	Data = Buffer;

	return Counter;
}

int KLString::Replace(const KLString& Old, const KLString& New, bool All)
{
	if (Capacity < Old.Capacity) return 0;

	int Counter = 0;
	int Found = -1;

	while ((Found = Find(Old)) != -1)
	{
		char* Buffer = new char[Capacity - Old.Capacity + New.Capacity + 1];

		memcpy(Buffer, Data, Found);
		memcpy(Buffer + Found, New.Data, New.Capacity);
		memcpy(Buffer + Found + New.Capacity, Data + Found + Old.Capacity, Capacity - Found - Old.Capacity);

		delete [] Data;

		Data = Buffer;

		Capacity -= Old.Capacity - New.Capacity;

		Counter++;

		if (!All) break;
	}

	return Counter;
}

int KLString::Count(const KLString& String) const
{
	int Steps = Capacity - String.Capacity;
	int Counter = 0;

	for (int i = 0; i < Steps; i++)
	{
		if (!strncmp(Data + i, String.Data, String.Capacity))
		{
			i += String.Capacity;

			Counter++;
		}
	}

	return Counter;
}

int KLString::Find(const KLString& String) const
{
	int Steps = Capacity - String.Capacity;

	for (int i = 0; i < Steps; i++)
	{
		if (!strncmp(Data + i, String.Data, String.Capacity))
		{
			return i;
		}
	}

	return -1;
}

int KLString::Size(void) const
{
	return Capacity;
}

void KLString::Clean(void)
{
	if (Data)
	{
		delete [] Data;

		Data = nullptr;
		Capacity = 0;
	}
}

int KLString::ToInt(void) const
{
	return atoi(Data);
}

double KLString::ToNumber(void) const
{
	return atof(Data);
}


char& KLString::operator[] (int ID)
{
	if (Capacity > ID)
		return Data[ID];
	else
		return *((char*) nullptr);
}

char KLString::operator[] (int ID) const
{
	if (Capacity > ID)
		return Data[ID];
	else
		return 0;
}

bool KLString::operator== (const KLString& String) const
{
	if (this == &String)
		return true;
	else
		return !strcmp(Data, String.Data);
}

bool KLString::operator!= (const KLString& String) const
{
	if (this == &String)
		return false;
	else
		return strcmp(Data, String.Data);
}

bool KLString::operator> (const KLString& String) const
{
	return strcmp(Data, String.Data) < 0;
}

bool KLString::operator< (const KLString& String) const
{
	return strcmp(Data, String.Data) > 0;
}

KLString KLString::operator+ (const KLString& String) const
{
	KLString Buffer;

	Buffer.Insert(Data);
	Buffer.Insert(String.Data);

	return Buffer;
}

KLString& KLString::operator= (const KLString& String)
{
	if (this == &String) return *this;

	Clean();

	Insert(String.Data);

	return *this;
}

KLString& KLString::operator+= (const KLString& String)
{
	Insert(String.Data);

	return *this;
}

KLString::operator const char* (void) const
{
	return Data;
}

template KLString::KLString(const long double&, const char*);
template KLString::KLString(const double&, const char*);
template KLString::KLString(const float&, const char*);

template KLString::KLString(const long unsigned&, const char*);
template KLString::KLString(const unsigned&, const char*);
template KLString::KLString(const short unsigned&, const char*);

template KLString::KLString(const long long int&, const char*);
template KLString::KLString(const long int&, const char*);
template KLString::KLString(const int&, const char*);
template KLString::KLString(const short int&, const char*);
