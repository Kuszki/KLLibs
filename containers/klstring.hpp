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

#ifndef KLSTRING_HPP
#define KLSTRING_HPP

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*! \brief	Lekka interpretacja łańcucha znaków.
 *
 * Prosta i lekka interpretacja łańcucha znaków. Wymaga jedynie kilku podstawowych funkcji biblioteki `string.h`.
 *
 */
class KLString
{

	protected:

		char* Data;	//!< Wskaźnik na przechowywane dane.

		int Capacity;	//!< Ilość przechowywanych danych.

	public:

		/*! \brief		Konstruktor konwertujący z typu `double`.
		 *  \param [in]	Value Wybrana liczba.
		 *
		 * Tworzy obiekt konwertując liczbę do postaci łańcucha znaków.
		 *
		 */
		KLString(double Value);

		/*! \brief		Konstruktor konwertujący z typu `int`.
		 *  \param [in]	Value Wybrana liczba.
		 *
		 * Tworzy obiekt konwertując liczbę do postaci łańcucha znaków.
		 *
		 */
		KLString(int Value);

		/*! \brief		Konstruktor konwertujący z `char`.
		 *  \param [in]	Char Wybrany znak.
		 *
		 * Tworzy obiekt o jednej wybranej literze.
		 *
		 */
		KLString(char Char);

		/*! \brief		Konstruktor konwertujący z typu `const void*`.
		 *  \param [in]	Value Wybrany adres.
		 *
		 * Tworzy obiekt konwertując liczbę do postaci łańcucha znaków.
		 *
		 */
		KLString(const void* Value);

		/*! \brief		Konstruktor konwertujący z `const char*`.
		 *  \param [in]	String Wybrany łańcuch znaków.
		 *
		 * Tworzy obiekt na bazie podanego łańcucha kopiując go do pamięci.
		 *
		 */
		KLString(const char* String);

		/*! \brief		Konstruktor kopiujący.
		 *  \param [in]	String Obiekt do skopiowania.
		 *
		 * Tworzy nowy obiekt na podstawie podanego obiektu.
		 *
		 */
		KLString(const KLString& String);

		/*! \brief		Konstruktor domyślny.
		 *
		 * Inicjuje wszystkie pola obiektu.
		 *
		 */
		KLString(void);

		/*! \brief		Destruktor.
		 *
		 * Zwalnia wszystkie użyte zasoby.
		 *
		 */
		~KLString(void);

		/*! \brief		Wstawianie łańcucha.
		 *  \param [in]	String	Łańcuch do wstawienia
		 *  \param [in]	Position	Pozycja łańcucha - numerowane od 0, -1 oznacza koniec bierzącego łańcucha.
		 *  \return		Aktualna liczba znaków.
		 *
		 * Dodaje do łańcucha podany łańcuch.
		 *
		 */
		int Insert(const KLString& String,
				 int Position = -1);

		/*! \brief		Usuwanie łańcucha.
		 *  \param [in]	String	Łańcuch do usunięcia
		 *  \param [in]	All		Usuń wszystkie wystąpienia.
		 *  \return		Ilość usunięć.
		 *
		 * Usuwa z łańcucha wybrany łańcuch.
		 *
		 */
		int Delete(const KLString& String,
				 bool All = false);

		/*! \brief		Zamiana słów.
		 *  \param [in]	Old	Łańcuch do usunięcia.
		 *  \param [in]	New	Łańcuch do wstawienia.
		 *  \param [in]	All	Zamień wszystkie wystąpienia.
		 *  \return		Ilość zmienionych fraz.
		 *
		 * Usuwa z łańcucha wybrany łańcuch i wstawia na jego miejsce nowy.
		 *
		 */
		int Replace(const KLString& Old,
				  const KLString& New,
				  bool All = false);

		/*! \brief		Zliczanie wystąpień.
		 *  \param [in]	String Łańcuch do wyszukania
		 *  \return		Ilość wystąpień.
		 *
		 * Szuka w łańcuchu wybranejfrazy i zwraca ilość wystąpień.
		 *
		 */
		int Count(const KLString& String) const;

		/*! \brief		Wyszukiwanie frazy.
		 *  \param [in]	String Łańcuch do wyszukania
		 *  \return		Miejsce wystąpienia numerowane od zera lub -1 gdy nic nie znaleziono.
		 *
		 * Szuka w łańcuchu wybranejfrazy i zwraca miejsce pierwszego wystąpienia.
		 *
		 */
		int Find(const KLString& String) const;


		/*! \brief		Długość łańcucha.
		 *  \return		Liczba znaków w łańcuchu.
		 *
		 * Zwraca ilość znaków przechowywaną w łańcuchu.
		 *
		 */
		int Size(void) const;

		/*! \brief		Czyszczenie łańcucha.
		 *
		 * Usuwa wszystkie znaki z łańcucha.
		 *
		 */
		void Clean(void);

		/*! \brief		Operator konwersji na `long int`.
		 *  \return		Interpretacja łańcucha jako liczba całkowita.
		 *
		 * Zwraca wynik funkcji `atol` użytej na łańcuchu.
		 *
		 */
		int ToInt(void) const;

		/*! \brief		Operator konwersji na `double`.
		 *  \return		Interpretacja łańcucha jako liczba zmiennoprzecinkowa.
		 *
		 * Zwraca wynik funkcji `atof` użytej na łańcuchu.
		 *
		 */
		double ToNumber(void) const;

		/*! \brief		Wybór znaku.
		 *  \param [in]	ID Pozycja znaku.
		 *  \return		Referencja do wybranego znaku.
		 *  \warning		Gdy wybrany znak nie istnieje to zwracana jest referencja na pusty wskaźnik, co zapewne wywoła krytyczny wyjątek.
		 *
		 * Wybiera z łańcucha znak o podaner pozycji indeksowanej od zera.
		 *
		 */
		char& operator[] (int ID);

		/*! \brief		Wybór znaku.
		 *  \param [in]	ID Pozycja znaku.
		 *  \return		Kopia wybranego znaku.
		 *  \note			Gdy wybrany znak nie istnieje to zwracany jest terminator `0`.
		 *
		 * Wybiera z łańcucha znak o podaner pozycji indeksowanej od zera.
		 *
		 */
		char operator[] (int ID) const;

		/*! \brief		Sprawdza czy łańcuchy są jednakowe.
		 *  \param [in]	String Łańcuch do porównania.
		 *  \return		`true` jeśli łańcuchy są jednakowe, lub `false` gdy są różne.
		 *
		 * Dokonuje porównania łańcuchów i zwraca rezultat operacji.
		 *
		 */
		bool operator== (const KLString& String) const;

		/*! \brief		Sprawdza czy łańcuchy są różne.
		 *  \param [in]	String Łańcuch do porównania.
		 *  \return		`false` jeśli łańcuchy są jednakowe, lub `true` gdy są różne.
		 *
		 * Dokonuje porównania łańcuchów i zwraca rezultat operacji.
		 *
		 */
		bool operator!= (const KLString& String) const;

		/*! \brief		Sprawdza czy łańcuch jest większy od podanego.
		 *  \param [in]	String Łańcuch do porównania.
		 *  \return		`true` jeśli łańcuch jest większy od podanego, lub `false` gdy nie.
		 *
		 * Dokonuje porównania łańcuchów i zwraca rezultat operacji.
		 *
		 */
		bool operator> (const KLString& String) const;

		/*! \brief		Sprawdza czy łańcuch jest mniejszy od podanego.
		 *  \param [in]	String Łańcuch do porównania.
		 *  \return		`false` jeśli łańcuch jest większy od podanego, lub `true` gdy nie.
		 *
		 * Dokonuje porównania łańcuchów i zwraca rezultat operacji.
		 *
		 */
		bool operator< (const KLString& String) const;

		/*! \brief		Dodaje łańcuchy.
		 *  \param [in]	String Łańcuch do dodania.
		 *  \return		Nowy obiekt reprezentujący sumę łańcuchów.
		 *
		 * Tworzy nowy łańcuch jako sumę dwóch łańcuchów.
		 *
		 */
		KLString operator+ (const KLString& String) const;

		/*! \brief		Operator przypisania.
		 *  \param [in]	String Łańcuch do przypisania.
		 *  \return		Referencja do bierzącego obiektu.
		 *
		 * Kopiuje podany łańcuch i zastępuje bierzący kopią podanego.
		 *
		 */
		KLString& operator= (const KLString& String);

		/*! \brief		Operator przypisania z dodawaniam.
		 *  \param [in]	String Łańcuch do dodania.
		 *  \return		Referencja do bierzącego obiektu.
		 *
		 * Dodaje podany łańcuch do bierzącego obiektu.
		 *
		 */
		KLString& operator+= (const KLString& String);

		/*! \brief		Operator konwersji na `const char*`.
		 *  \return		Stały wskaźnik na dane obiektu.
		 *
		 * Zwraca stały wskaźnik na miejsce w którym przechowywany jest łańcuch.
		 *
		 */
		operator const char* (void) const;

};

#endif // KLSTRING_HPP
