/***********************************************************************
 *
 *  Lightweight String interpretation for KLLibs
 *  Copyright (C) 2015  Łukasz "Kuszki" Dróżdż
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses/.
 *
 **********************************************************************/

#ifndef KLSTRING_HPP
#define KLSTRING_HPP

#include "../libbuild.hpp"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define S (KLString)	//!< Makro konwertujące na KLString.

/*! \file		klstring.hpp
 *  \brief	Deklaracje dla klasy KLString i jej składników.
 *
 */

/*! \file		klstring.cpp
 *  \brief	Implementacja klasy KLString i jej składników.
 *
 */

/*! \brief	Lekka interpretacja łańcucha znaków.
 *
 * Prosta i lekka interpretacja łańcucha znaków. Wymaga jedynie kilku podstawowych funkcji biblioteki `string.h`.
 *
 */
class EXPORT KLString
{

	protected:

		char* Data;		//!< Wskaźnik na przechowywane dane.

		int Capacity;		//!< Ilość przechowywanych danych.

		size_t Reserved;	//!< Ilość zarezerwowanych danych.

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

		/*! \brief		Konstruktor konwertujący z `bool`.
		 *  \param [in]	Bool Wartość logiczna.
		 *
		 * Tworzy obiekt na podstawie wartości logicznej.
		 *
		 */
		KLString(bool Bool);

		/*! \brief		Konstruktor konwertujący z typu `const void*`.
		 *  \param [in]	Value Wybrany adres.
		 *
		 * Tworzy obiekt konwertując liczbę do postaci łańcucha znaków.
		 *
		 */
		KLString(const void* Value);

		/*! \brief		Konstruktor konwertujący z `char`.
		 *  \param [in]	Char Wybrany znak.
		 *
		 * Tworzy obiekt o jednej wybranej literze.
		 *
		 */
		KLString(char Char);

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

		/*! \brief		Konstruktor przenoszący.
		 *  \param [in]	String Obiekt do przeniesienia.
		 *
		 * Tworzy nowy obiekt na podstawie podanego obiektu bez niepotrzebnych alokacji.
		 *
		 */
		KLString(KLString&& String);

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

		/*! \brief		Rezerwacja miejsca na dane.
		 *  \param [in]	Size Liczba bajtów do zarezerwowania.
		 *  \warning		Przed rozpoczęciem stałych operacji na łańcuchu użyj metody `Refresh()`.
		 *  \warning		Przed rozpoczęciem operacji modyfikujących łańcuch użyj metody `Finalize()`.
		 *  \see			Finalize(), Refresh().
		 *
		 * Czyści łańcuch i rezerwuje wybraną ilość bajtów na znaki. Znaki można wprowadzać za pomocą operatora wyłuskania. Po zarezerwowaniu wszystkie znaki mają wartość `0`.
		 *
		 */
		void Reserve(size_t Size);

		/*! \brief		Odświeża informację o łańcuchu.
		 *  \note			Dotyczy tylko łańcuchów z zarezerwowanym miejscem.
		 *  \see			Reserve().
		 *
		 * Oblicza na nowo ilość znaków w łańcuchu.
		 *
		 */
		void Refresh(void);

		/*! \brief		Wymazuje wpisane znaki.
		 *  \note			Dotyczy tylko łańcuchów z zarezerwowanym miejscem.
		 *  \see			Reserve().
		 *
		 * Wymazuje dane z zarezerwowanej pamięci.
		 *
		 */
		void Erase(void);

		/*! \brief		Przekształca łańcuch w standardowy.
		 *  \note			Dotyczy tylko łańcuchów z zarezerwowanym miejscem.
		 *  \see			Reserve().
		 *
		 * Oblicza aktualną długość łańcucha i zwalnia nieużywaną pamięć. Umożliwia bezpieczne wywołanie innych metod.
		 *
		 */
		void Finalize(void);

		/*! \brief		Wstawianie łańcucha.
		 *  \param [in]	String	Łańcuch do wstawienia.
		 *  \param [in]	Position	Pozycja łańcucha - numerowane od 0, -1 oznacza koniec bierzącego łańcucha.
		 *  \return		Aktualna liczba znaków.
		 *
		 * Dodaje do łańcucha podany łańcuch.
		 *
		 */
		int Insert(const KLString& String,
				 int Position = -1);

		/*! \brief		Wstawianie łańcucha.
		 *  \param [in]	String	Łańcuch do wstawienia
		 *  \param [in]	Position	Pozycja łańcucha - numerowane od 0, -1 oznacza koniec bierzącego łańcucha.
		 *  \param [in]	Length	Długość łańcucha do dodania.
		 *  \return		Aktualna liczba znaków.
		 *  \warning		Niepoprawna długość łańcucha może przynieść niezamierzone konsekwencje.
		 *
		 * Dodaje do łańcucha podany łańcuch. Gdy długość nie zostanie podana to zostanie ona automatycznie obliczona.
		 *
		 */
		int Insert(const char* String,
				 int Position = -1,
				 int Length = -1);

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

		/*! \brief		Usuwanie części łańcucha.
		 *  \param [in]	Start	Punkt początkowy.
		 *  \param [in]	Stop		Punkt końcowy.
		 *  \return		Ilość usunięć.
		 *
		 * Usuwa z łańcucha wybrany fragment.
		 *
		 */
		int Delete(int Start,
				 int Stop);

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
		 *  \param [in]	String	Łańcuch do wyszukania.
		 *  \param [in]	Start	Początek wyszukiwania.
		 *  \param [in]	Stop		Koniec wyszukiwania.
		 *  \return		Ilość wystąpień.
		 *
		 * Szuka w łańcuchu wybranejfrazy i zwraca ilość wystąpień.
		 *
		 */
		int Count(const KLString& String,
				int Start = 0,
				int Stop = 0) const;

		/*! \brief		Wyszukiwanie frazy.
		 *  \param [in]	String	Łańcuch do wyszukania.
		 *  \param [in]	Start	Początek wyszukiwania.
		 *  \param [in]	Stop		Koniec wyszukiwania.
		 *  \return		Miejsce wystąpienia numerowane od zera lub -1 gdy nic nie znaleziono.
		 *
		 * Szuka w łańcuchu wybranejfrazy i zwraca miejsce pierwszego wystąpienia.
		 *
		 */
		int Find(const KLString& String,
			    int Start = 0,
			    int Stop = 0) const;

		/*! \brief		Kopia części łańcucha.
		 *  \param [in]	Start	Początek ciągu.
		 *  \param [in]	Stop		Koniec ciągu.
		 *  \return		Łańcuch złożony z części obejmującej podany zakres.
		 *
		 * Kopiuje znaki od wybranego znaku początkowego do końcowego i zwraca nowy łańcuch.
		 *
		 */
		KLString Part(int Start,
				    int Stop) const;

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

		/*! \brief		Operator konwersji na `bool`.
		 *  \return		Interpretacja łańcucha jako wartość logiczna.
		 *
		 * Zwraca `true` gdy ciąg jest liczbą niezerową lub ma wartość "true".
		 *
		 */
		int ToBool(void) const;

		/*! \brief		Operator konwersji na `int`.
		 *  \return		Interpretacja łańcucha jako liczba całkowita.
		 *
		 * Zwraca wynik funkcji `atoi` użytej na łańcuchu.
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

		/*! \brief		Sprawdza czy łańcuchy są jednakowe.
		 *  \param [in]	String Łańcuch do porównania.
		 *  \return		`true` jeśli łańcuchy są jednakowe, lub `false` gdy są różne.
		 *
		 * Dokonuje porównania łańcuchów i zwraca rezultat operacji.
		 *
		 */
		bool operator== (const char* String) const;

		/*! \brief		Sprawdza czy łańcuchy są różne.
		 *  \param [in]	String Łańcuch do porównania.
		 *  \return		`false` jeśli łańcuchy są jednakowe, lub `true` gdy są różne.
		 *
		 * Dokonuje porównania łańcuchów i zwraca rezultat operacji.
		 *
		 */
		bool operator!= (const char* String) const;

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

		/*! \brief		Dodaje łańcuchy.
		 *  \param [in]	String Łańcuch do dodania.
		 *  \return		Nowy obiekt reprezentujący sumę łańcuchów.
		 *
		 * Tworzy nowy łańcuch jako sumę dwóch łańcuchów.
		 *
		 */
		KLString operator+ (const char* String) const;

		/*! \brief		Operator przypisania.
		 *  \param [in]	String Łańcuch do przypisania.
		 *  \return		Referencja do bierzącego obiektu.
		 *
		 * Kopiuje podany łańcuch i zastępuje bierzący kopią podanego.
		 *
		 */
		KLString& operator= (const KLString& String);

		/*! \brief		Operator przeniesienia.
		 *  \param [in]	String Łańcuch do przeniesienia.
		 *  \return		Referencja do bierzącego obiektu.
		 *
		 * Przenosi podany łańcuch i zastępuje bierzący podanym.
		 *
		 */
		KLString& operator= (KLString&& String);

		/*! \brief		Operator przypisania z dodawaniam.
		 *  \param [in]	String Łańcuch do dodania.
		 *  \return		Referencja do bierzącego obiektu.
		 *
		 * Dodaje podany łańcuch do bierzącego obiektu.
		 *
		 */
		KLString& operator+= (const KLString& String);

		/*! \brief		Operator przypisania z dodawaniam.
		 *  \param [in]	String Łańcuch do dodania.
		 *  \return		Referencja do bierzącego obiektu.
		 *
		 * Dodaje podany łańcuch do bierzącego obiektu.
		 *
		 */
		KLString& operator+= (const char* String);

		/*! \brief		Operator konwersji na `const char*`.
		 *  \return		Stały wskaźnik na dane obiektu.
		 *
		 * Zwraca stały wskaźnik na miejsce w którym przechowywany jest łańcuch.
		 *
		 */
		operator const char* (void) const;

		/*! \brief		Operator konwersji na `bool`.
		 *  \return		Stan łańcucha.
		 *
		 * Zwraca `true` gdy łańcuch zawiera znaki lub `false` gdy jest pusty.
		 *
		 */
		explicit operator bool (void) const;

};

#endif // KLSTRING_HPP
