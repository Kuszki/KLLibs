/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Lightweight String interpretation for KLLibs                           *
 *  Copyright (C) 2015  Łukasz "Kuszki" Dróżdż  l.drozdz@openmailbox.org   *
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

#include "klstring.hpp"

KLString::KLString(double Value)
: KLString()
{
	char Buffer[32];

#ifndef F_CPU
	snprintf(Buffer, 32, "%f", Value);
#else
	dtostrf(Value, 0, 5, Buffer);
#endif

	Capacity	= strlen(Buffer);
	Data		= (char*) malloc(Capacity + 1);

	memcpy(Data, Buffer, Capacity + 1);
}

KLString::KLString(int Value)
: KLString()
{
	char Buffer[32];

#ifndef F_CPU
	snprintf(Buffer, 32, "%i", Value);
#else
	itoa(Value, Buffer, 10);
#endif

	Capacity	= strlen(Buffer);
	Data		= (char*) malloc(Capacity + 1);

	memcpy(Data, Buffer, Capacity + 1);
}

KLString::KLString(bool Bool)
: KLString()
{
	static const char* Buffer[] = {"true", "false"};

	Capacity	= Bool ? 4 : 5;
	Data		= (char*) malloc(Capacity + 1);

	memcpy(Data, Buffer, Capacity + 1);
}

KLString::KLString(const void* Value)
: KLString()
{
	char Buffer[17];

#ifndef F_CPU
	snprintf(Buffer, 17, "%p", Value);
#else
	utoa((unsigned) Value, Buffer, 17);
#endif

	Capacity	= strlen(Buffer);
	Data		= (char*) malloc(Capacity + 1);

	memcpy(Data, Buffer, Capacity + 1);
}

KLString::KLString(char Char)
: KLString()
{
	Data		= (char*) malloc(2);

	Data[0]	= Char;
	Data[1]	= 0;

	Capacity	= 1;
}

KLString::KLString(const char* String)
: KLString()
{
	if (String)
	{
		Capacity	= strlen(String);
		Data		= (char*) malloc(Capacity + 1);

		memcpy(Data, String, Capacity + 1);
	}
}

KLString::KLString(const KLString& String)
: Data(nullptr), Capacity(String.Capacity), Reserved(0)
{
	if (Capacity)
	{
		Data = (char*) malloc(Capacity + 1);

		memcpy(Data, String.Data, Capacity + 1);
	}
}

KLString::KLString(KLString&& String)
: Data(String.Data), Capacity(String.Capacity), Reserved(String.Reserved)
{
	String.Data		= nullptr;
	String.Capacity	= 0;
	String.Reserved	= 0;
}

KLString::KLString(void)
: Data(nullptr), Capacity(0), Reserved(0) {}

KLString::~KLString(void)
{
	if (Data) free(Data);
}

void KLString::Reserve(size_t Size)
{
	Reserved = Size + 1;
	Data = (char*) realloc(Data, Reserved);

	Erase();
}

void KLString::Refresh(void)
{
	if (Reserved) Capacity = strlen(Data);
}

void KLString::Erase(void)
{
	if (Reserved) memset(Data, 0, Reserved);
}

void KLString::Finalize(void)
{
	if (Reserved)
	{
		Capacity = strlen(Data);
		Data = (char*) realloc(Data ,Capacity + 1);

		Reserved = 0;
	}
}

int KLString::Insert(const KLString& String, int Position)
{
	return Insert(String.Data, Position, String.Capacity);
}

int KLString::Insert(const char* String, int Position, int Length)
{
	if (!String || Position > Capacity) return -1;

	const int Strlen = (Length > 0) ? Length : strlen(String);

	if (!Data)
	{
		Data = (char*) malloc(Capacity + Strlen + 1);

		memcpy(Data, String, Strlen + 1);
	}
	else if (Position < 0 || Position == Capacity)
	{
		Data = (char*) realloc(Data, Capacity + Strlen + 1);

		memcpy(Data + Capacity, String, Strlen + 1);
	}
	else
	{
		char* Buffer = (char*) malloc(Capacity + Strlen + 1);

		memcpy(Buffer, Data, Position);
		memcpy(Buffer + Position, String, Strlen);
		memcpy(Buffer + Position + Strlen, Data + Position, Capacity - Position + 1);

		free(Data);

		Data = Buffer;
	}

	return Capacity += Strlen;
}

int KLString::Insert(char Char, int Position)
{
	if (!Char || Position > Capacity) return -1;

	if (!Data)
	{
		Data = (char*) malloc(2);

		Data[0] = Char;
		Data[1] = 0;
	}
	else if (Position < 0 || Position == Capacity)
	{
		Data = (char*) realloc(Data, Capacity + 2);

		Data[Capacity] = Char;
		Data[Capacity + 1] = 0;
	}
	else
	{
		Data = (char*) realloc(Data, Capacity + 2);

		memcpy(Data + Position + 1, Data + Position, Capacity - Position + 1);

		Data[Position] = Char;
	}

	return ++Capacity;
}

int KLString::Delete(const KLString& String, bool All)
{
	if (Capacity < String.Capacity) return 0;

	int Counter	= 0;
	int Found		= -1;

	while ((Found = Find(String)) != -1)
	{
		memcpy(Data + Found, Data + String.Capacity, Capacity - Found + 1);

		Capacity -= String.Capacity;

		Counter++;

		if (!All) break;
	}

	Data = (char*) realloc(Data, Capacity + 1);

	return Counter;
}

int KLString::Delete(int Start, int Stop)
{
	if (Stop > Capacity) Stop = Capacity;

	if (Stop <= Start) return 0;

	int NewCap	= Capacity - Stop + Start - 1;
	int OldCap	= Capacity;

	char* Buffer = (char*) malloc(NewCap + 1);

	memcpy(Buffer, Data, Start);
	memcpy(Buffer + Start, Data + Stop + 1, OldCap - Stop - 1);

	Buffer[NewCap] = 0;

	free(Data);

	Data = Buffer;

	Capacity = NewCap;

	return OldCap - NewCap;
}

int KLString::Replace(const KLString& Old, const KLString& New, bool All, bool Words)
{
	if (Capacity < Old.Capacity) return 0;

	int Counter	= 0;
	int Found		= -1;

	while ((Found = Find(Old, 0, 0, Words)) != -1)
	{
		char* Buffer = (char*) malloc(Capacity - Old.Capacity + New.Capacity + 1);

		memcpy(Buffer, Data, Found);
		memcpy(Buffer + Found, New.Data, New.Capacity);
		memcpy(Buffer + Found + New.Capacity, Data + Found + Old.Capacity, Capacity - Found - Old.Capacity);

		free(Data);

		Capacity -= Old.Capacity - New.Capacity;

		Data = Buffer;
		Data[Capacity] = 0;

		Counter++;

		if (!All) break;
	}

	return Counter;
}

int KLString::Count(const KLString& String, int Start, int Stop, bool Words) const
{
	if (!Data || !String.Data) return 0;

	int Counter = 0;

	while (Start = Find(String, Start, Stop, Words) + 1)
	{
		++Counter;
	}

	return Counter;
}

int KLString::Find(const KLString& String, int Start, int Stop, bool Words) const
{
	Stop = ((Stop && Stop < Capacity) ? Stop : Capacity) - String.Capacity + 1;

	for (int i = Start; i < Stop; i++)
	{
		if (!strncmp(Data + i, String.Data, String.Capacity))
		{
			if (Words)
			{
				bool Found = true;

				if (i) Found = Found && !isalnum(Data[i - 1]);
				if (i + String.Capacity < Capacity) Found = Found && !isalnum(Data[i + String.Capacity]);

				if (Found) return i;
			}
			else return i;
		}
	}

	return -1;
}

KLString KLString::Part(int Start, int Stop) const
{
	if (Start >= Stop || Start > Capacity || Stop > Capacity) return KLString();

	KLString Buffer;

	Buffer.Capacity = Stop - Start;
	Buffer.Data = (char*) malloc(Buffer.Capacity + 1);

	memcpy(Buffer.Data, Data + Start, Buffer.Capacity);

	Buffer.Data[Buffer.Capacity] = 0;

	return Buffer;
}

char& KLString::First(void)
{
	return Data[0];
}

char KLString::First(void) const
{
	if (Capacity) return Data[0];
	else return 0;
}

char& KLString::Last(void)
{
	return Data[Capacity - 1];
}

char KLString::Last(void) const
{
	if (Capacity) return Data[Capacity - 1];
	else return 0;
}

int KLString::Size(void) const
{
	return Capacity;
}

void KLString::Clean(void)
{
	if (Data)
	{
		free(Data);

		Capacity	= 0;
		Data		= nullptr;
	}
}

bool KLString::ToBool(void) const
{
	if (Data) return atoi(Data) || !strcmp(Data, "true") || !strcmp(Data, "TRUE");
	else return false;
}

int KLString::ToInt(void) const
{
	if (Data) return atoi(Data);
	else return 0;
}

double KLString::ToNumber(void) const
{
	if (Data) return atof(Data);
	else return 0.0;
}


char& KLString::operator[] (int ID)
{
	return Data[ID];
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
		return !strcmp(Data ? Data : "", String.Data ? String.Data : "");
}

bool KLString::operator!= (const KLString& String) const
{
	if (this == &String)
		return false;
	else
		return strcmp(Data ? Data : "", String.Data ? String.Data : "");
}

bool KLString::operator== (const char* String) const
{
	if (Data == String)
		return true;
	else
		return !strcmp(Data ? Data : "", String);
}

bool KLString::operator!= (const char* String) const
{
	return strcmp(Data ? Data : "", String);
}

bool KLString::operator> (const KLString& String) const
{
	return strcmp(Data ? Data : "", String.Data ? String.Data : "") < 0;
}

bool KLString::operator< (const KLString& String) const
{
	return strcmp(Data ? Data : "", String.Data ? String.Data : "") > 0;
}

KLString KLString::operator+ (const KLString& String) const
{
	KLString Buffer;

	Buffer.Insert(Data);
	Buffer.Insert(String.Data);

	return Buffer;
}

KLString KLString::operator+ (const char* String) const
{
	KLString Buffer;

	Buffer.Insert(Data);
	Buffer.Insert(String);

	return Buffer;
}

KLString KLString::operator+ (char Char) const
{
	KLString Buffer;

	Buffer.Insert(Data);
	Buffer.Insert(Char);

	return Buffer;
}

KLString& KLString::operator= (const KLString& String)
{
	if (this == &String) return *this;

	Clean();

	Insert(String.Data);

	return *this;
}

KLString& KLString::operator= (KLString&& String)
{
	Clean();

	Data = String.Data;

	Capacity = String.Capacity;

	String.Data = nullptr;

	return *this;
}

KLString& KLString::operator+= (const KLString& String)
{
	Insert(String.Data);

	return *this;
}

KLString& KLString::operator+= (const char* String)
{
	Insert(String);

	return *this;
}

KLString::operator const char* (void) const
{
	return Data;
}

KLString& KLString::operator << (const KLString& Input)
{
	Insert(Input);

	return *this;
}

KLString& KLString::operator << (const char* Input)
{
	Insert(Input);

	return *this;
}

KLString& KLString::operator << (char Input)
{
	Insert(Input);

	return *this;
}
