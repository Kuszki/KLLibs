/***********************************************************************
 *
 * Variables management system interpretation for KLLibs
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

#ifndef KLVARIABLES_HPP
#define KLVARIABLES_HPP

/*! \file		klvariables.hpp
 *  \brief	Deklaracje dla klasy KLVariables i jej składników.
 *
 */

/*! \file		klvariables.cpp
 *  \brief	Implementacja klasy KLVariables i jej składników.
 *
 */

#ifdef QT_VERSION
	#include "kllibs.hpp"
#else
	#define EXPORT
#endif

#include "containers/klmap.hpp"
#include "containers/klstring.hpp"

#ifndef size_t
	typedef unsigned int size;
#else
	typedef size_t size;
#endif

/*! \brief	System zarządzania zmiennymi dla skryptu.
 *
 * System zarządzania zmiennymi umożliwiający bindowanie lokalnych zmiennych. Ze względu na lekkość oferuje statyczne typowanie, umożliwia natomiast automatyczną konwersje.
 *
 * Możliwe jest definiowanie zmiennych z podziałem na zakresy tak, że można dublować nazwy zmiennych byle w jednym zakresie nazwy były unikatowe. Podczas przeszukiwania systemu gdy istnieje wyższy zakres i zmienna nie zostanie znaleziona w bierzącym systemie następuje rekursywne przeszukanie wyższych systemów.
 *
 * Organizacja obsługuje możliwość iteracji po zakresie jedynie po bierzącym poziomie, zgodnie z `KLMap`.
 *
 */
class EXPORT KLVariables
{

	/*! \brief		Wyliczenie typu zmiennej.
	 *
	 * Definiuje typ zmiennej przechowywanej w organizacji.
	 *
	 */
	public: enum TYPE
	{
		BOOLEAN,	//!< Łańcuch znaków.
		NUMBER,	//!< Typ zmiennoprzecinkowy.
		INTEGER	//!< Typ całkowity.
	};

	/*! \brief		Reprezentacja pojedynczej zmiennej.
	 *
	 * Definiuje typ zmiennej przechowywanej w organizacji.
	 *
	 */
	private: class KLVariable
	{

		protected:

			void* Pointer;		//!< Wskaźnik na dane.

		public:

			const bool Binded;	//!< Obecność bindu.

			const TYPE Type;	//!< Wyliczenie typu zmiennej.

			/*! \brief		Konstruktor kopiujący.
			 *  \param [in]	Object Obiekt do sklonowania.
			 *
			 * Kopiuje dane zmiennej i tworzy nową na jej wzór. Gdy zmienna jest bindem konstruktor ten klonuje bind.
			 *
			 */
			KLVariable(const KLVariable& Object);

			/*! \brief		Domyślny konstruktor.
			 *  \param [in]	VarType	Wyliczenie typu.
			 *  \param [in]	Bind		Adres przypisanej zmiennej.
			 *  \warning		Bardzo istotne jest, aby zachować zgodność podanego typu z typem zbindowanego obiektu.
			 *
			 * Tworzy zmienną na podstawie podanego typu i opcjonalnie binduje ją pod wskazany adres.
			 *
			 */
			KLVariable(TYPE VarType = NUMBER,
					 void* Bind = nullptr);

			/*! \brief		Destruktor.
			 *
			 * Niszczy obiekt o ile nie jest on bindem i zwalnia zasoby.
			 *
			 */
			~KLVariable(void);

			/*! \brief		Konwersja na `KLString`.
			 *  \return		Reprezentacja zmiennej jako `KLString`.
			 *
			 * Konwertuje obiekt do `KLString` i zwraca wynik.
			 *
			 */
			KLString ToString(void) const;

			/*! \brief		Konwersja na `double`.
			 *  \return		Reprezentacja zmiennej jako `double`.
			 *
			 * Konwertuje obiekt do `double` i zwraca wynik.
			 *
			 */
			double ToNumber(void) const;

			/*! \brief		Konwersja na `int`.
			 *  \return		Reprezentacja zmiennej jako `int`.
			 *
			 * Konwertuje obiekt do `int` i zwraca wynik.
			 *
			 */
			int ToInt(void) const;

			/*! \brief		Konwersja na `bool`.
			 *  \return		Reprezentacja zmiennej jako `bool`.
			 *
			 * Konwertuje obiekt do `bool` i zwraca wynik.
			 *
			 */
			int ToBool(void) const;

			/*! \brief		Zmiana wartość zmiennej.
			 *  \param [in]	String Łańcuch do przypisania.
			 *
			 * Przypisuje do obiektu odpowiednią reprezentacje podanego obiektu po dokonaniu konwersji.
			 *
			 */
			void Set(const KLString& String);

			/*! \brief		Zmiana wartość zmiennej.
			 *  \param [in]	Number Liczba zmiennoprzecinkowa.
			 *
			 * Przypisuje do obiektu odpowiednią reprezentacje podanego obiektu po dokonaniu konwersji.
			 *
			 */
			void Set(double Number);

			/*! \brief		Zmiana wartość zmiennej.
			 *  \param [in]	Integer Liczba całkowita.
			 *
			 * Przypisuje do obiektu odpowiednią reprezentacje podanego obiektu po dokonaniu konwersji.
			 *
			 */
			void Set(int Integer);

			/*! \brief		Operator przypisania.
			 *  \param [in]	Object Obiekt do sklonowania.
			 *
			 * Przypisuje do obiektu odpowiednią reprezentacje podanego obiektu po dokonaniu konwersji. Nie zmienia bindów i nie dokonuje bindowania.
			 *
			 */
			KLVariable& operator= (const KLVariable& Object);

	};

	protected:

		KLMap<KLVariable, KLString> Variables;	//!< Mapa zmiennych.

	public:

		KLVariables* const Parent;			//!< Zmienne wyższego zakresu.

		/*! \brief		Domyślny konstruktor.
		 *  \param [in]	UpperScoope System zmiennych wyższego zakresu.
		 *
		 * Tworzy nowy system zmiennych i opcjonalnie zapamiętuje wskaźnik na zmienne z wyższego zakresu.
		 *
		 */
		explicit KLVariables(KLVariables* UpperScoope = nullptr);

		/*! \brief		Konstruktor kopiujący.
		 *  \param [in]	Objects System zmiennych do skopiowania.
		 *
		 * Tworzy nowy system zmiennych kopiując istniejący. Kopiowanie zmiennych następuje zgodnie z regułami konstruktora `KLVariable::KLVariable(const KLVariable&)`.
		 *
		 */
		KLVariables(const KLVariables& Objects);

		/*! \brief		Tworzenie zmiennej w systemie.
		 *  \param [in]	Name		Nazwa zmiennej.
		 *  \param [in]	Object	Instancja zmiennej.
		 *  \return		Powodzenie operacji.
		 *
		 * Tworzy nową zmienną w systemie zgodnie z regułami konstruktora `KLVariable::KLVariable(const KLVariable&)`.
		 *
		 */
		bool Add(const KLString& Name,
			    const KLVariable& Object);

		/*! \brief		Tworzenie zmiennej w systemie.
		 *  \param [in]	Name		Nazwa zmiennej.
		 *  \param [in]	Type		Wyliczenie typu zmiennej.
		 *  \return		Powodzenie operacji.
		 *
		 * Tworzy nową zmienną w systemie zgodnie z regułami konstruktora `KLVariable::KLVariable(TYPE, void*)`.
		 *
		 */
		bool Add(const KLString& Name,
			    TYPE Type);

		/*! \brief		Tworzenie zmiennej w systemie.
		 *  \param [in]	Name		Nazwa zmiennej.
		 *  \param [in]	Boolean	Referencja do zbindowanego obiektu.
		 *  \return		Powodzenie operacji.
		 *
		 * Tworzy nową zmienną w systemie bindując do niej podany obiekt.
		 *
		 */
		bool Add(const KLString& Name,
			    bool& Boolean);

		/*! \brief		Tworzenie zmiennej w systemie.
		 *  \param [in]	Name		Nazwa zmiennej.
		 *  \param [in]	Number	Referencja do zbindowanego obiektu.
		 *  \return		Powodzenie operacji.
		 *
		 * Tworzy nową zmienną w systemie bindując do niej podany obiekt.
		 *
		 */
		bool Add(const KLString& Name,
			    double& Number);

		/*! \brief		Tworzenie zmiennej w systemie.
		 *  \param [in]	Name		Nazwa zmiennej.
		 *  \param [in]	Integer	Referencja do zbindowanego obiektu.
		 *  \return		Powodzenie operacji.
		 *
		 * Tworzy nową zmienną w systemie bindując do niej podany obiekt.
		 *
		 */
		bool Add(const KLString& Name,
			    int& Integer);

		/*! \brief		Usuwanie zmiennej z systemu.
		 *  \param [in]	Name Nazwa zmiennej.
		 *  \return		Powodzenie operacji.
		 *
		 * Usuwa wybraną zmienną z systemu zgodnie z regułami `KLVariable::~KLVariable()`.
		 *
		 */
		bool Delete(const KLString& Name);

		/*! \brief		Test obecności zmiennej.
		 *  \param [in]	Name Nazwa zmiennej.
		 *  \return		Obecność zmiennej.
		 *
		 * Sprawdza czy podana zmienna istnieje w systemie.
		 *
		 */
		bool Exists(const KLString& Name) const;

		/*! \brief		Pobranie ilości zmiennych.
		 *  \return		Ilośc zmiennych w obecnym zakresie.
		 *
		 * Sprawdza ile zmiennych istnieje w systemie.
		 *
		 */
		int Size(void) const;

		/*! \brief		Operator wyboru.
		 *  \param [in]	Name Nazwa zmiennej.
		 *  \return		Referencja do obiektu zmiennej.
		 *  \warning		Gdy podana zmienna nie istnieje zostają zastosowane reguły `KLMap::operator[]`.
		 *
		 * Wybiera z systemu zmienną o podanej nazwie.
		 *
		 */
		KLVariable& operator[] (const KLString& Name);

		/*! \brief		Operator wyboru.
		 *  \param [in]	Name Nazwa zmiennej.
		 *  \return		Stała eeferencja do obiektu zmiennej.
		 *  \warning		Gdy podana zmienna nie istnieje zostają zastosowane reguły `KLMap::operator[]`.
		 *
		 * Wybiera z systemu zmienną o podanej nazwie.
		 *
		 */
		const KLVariable& operator[] (const KLString& Name) const;

		KLMap<KLVariable, KLString>::KLMapVarIterator begin(void);
		KLMap<KLVariable, KLString>::KLMapVarIterator end(void);

		KLMap<KLVariable, KLString>::KLMapConstIterator begin(void) const;
		KLMap<KLVariable, KLString>::KLMapConstIterator end(void) const;

};

#endif // KLVARIABLES_HPP
