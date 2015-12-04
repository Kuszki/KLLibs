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

#ifndef KLVARIABLES_HPP
#define KLVARIABLES_HPP

#include "../libbuild.hpp"

#include "../containers/klmap.hpp"
#include "../containers/klstring.hpp"

#if defined(USING_BOOST)
#include <boost/function.hpp>
#include <boost/bind.hpp>
#endif


/*! \file		klvariables.hpp
 *  \brief	Deklaracje dla klasy KLVariables i jej składników.
 *
 */

/*! \file		klvariables.cpp
 *  \brief	Implementacja klasy KLVariables i jej składników.
 *
 */

/*! \brief	System zarządzania zmiennymi dla skryptu.
 *
 * System zarządzania zmiennymi umożliwiający bindowanie lokalnych zmiennych. Ze względu na lekkość oferuje statyczne typowanie, umożliwia natomiast automatyczną konwersje.
 *
 * Możliwe jest definiowanie zmiennych z podziałem na zakresy tak, że można dublować nazwy zmiennych byle w jednym zakresie nazwy były unikatowe. Podczas przeszukiwania systemu gdy istnieje wyższy zakres i zmienna nie zostanie znaleziona w bierzącym systemie następuje rekursywne przeszukanie wyższych systemów.
 *
 * Organizacja obsługuje możliwość iteracji po zakresie jedynie po bierzącym poziomie, zgodnie z `KLMap`.
 *
 */
class KLLIBS_EXPORT KLVariables
{

#if defined(USING_BOOST)
	public: using KLSCALLBACK = boost::function<void (double)>;
#else
	public: using KLSCALLBACK = void (*)(double);
#endif

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
	public: class KLLIBS_EXPORT KLVariable
	{

		protected:

			void* const Pointer;	//!< Adres bindu.

			double Variable;		//!< Przechowywane dane.

			bool Readonly;			//!< Modyfikator `tylko do odczytu` przy wykonywaniu skryptu.

			KLSCALLBACK Callback;	//!< Funkcja zwrotna przy zmianie stanu zmiennej.

		public:

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
			 *  \param [in]	Handler	Obiekt funkcyjny wywoływany przy modyfikacji zmiennej.
			 *  \param [in]	Writeable	Ustalenie możliwości edycji zmiennej.
			 *  \warning		Bardzo istotne jest, aby zachować zgodność podanego typu z typem zbindowanego obiektu.
			 *
			 * Tworzy zmienną na podstawie podanego typu i opcjonalnie binduje ją pod wskazany adres.
			 *
			 */
			KLVariable(TYPE VarType = NUMBER, void* Bind = nullptr, KLSCALLBACK Handler = KLSCALLBACK(), bool Writeable = true);

			/*! \brief		Konstruktor konwertujący z `bool`.
			 *  \param [in]	Boolean	Początkowa wartość.
			 *  \param [in]	Handler	Obiekt funkcyjny wywoływany przy modyfikacji zmiennej.
			 *  \param [in]	Writeable	Ustalenie możliwości edycji zmiennej.
			 *
			 * Tworzy zmienną logiczną o podanej wartości
			 *
			 */
			KLVariable(bool Boolean, KLSCALLBACK Handler = KLSCALLBACK(), bool Writeable = true);

			/*! \brief		Konstruktor konwertujący z `double`.
			 *  \param [in]	Number	Początkowa wartość.
			 *  \param [in]	Handler	Obiekt funkcyjny wywoływany przy modyfikacji zmiennej.
			 *  \param [in]	Writeable	Ustalenie możliwości edycji zmiennej.
			 *
			 * Tworzy zmienną rzeczywistą o podanej wartości
			 *
			 */
			KLVariable(double Number, KLSCALLBACK Handler = KLSCALLBACK(), bool Writeable = true);

			/*! \brief		Konstruktor konwertujący z `int`.
			 *  \param [in]	Integer	Początkowa wartość.
			 *  \param [in]	Handler	Obiekt funkcyjny wywoływany przy modyfikacji zmiennej.
			 *  \param [in]	Writeable	Ustalenie możliwości edycji zmiennej.
			 *
			 * Tworzy zmienną całkowitą o podanej wartości
			 *
			 */
			KLVariable(int Integer, KLSCALLBACK Handler = KLSCALLBACK(), bool Writeable = true);

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
			bool ToBool(void) const;

			/*! \brief		Sprawdzenie bindowania.
			 *  \return		Stan bindowania.
			 *
			 * Sprawdza czy obiekt jest bindem. Jeśli tak, to zwraca `true`, w przeciwnym razie zwraca `false`.
			 *
			 */
			bool Binded(void) const;

			/*! \brief		Ustalenie funkcji zwrotnej.
			 *  \param [in]	Handler Funkcja zwrotna.
			 *
			 * Ustala funkcję zwrotną wywoływaną przy zmianie wartości zmiennej.
			 *
			 */
			void SetCallback(KLSCALLBACK Handler);

			/*! \brief		Ustalenie funkcji zwrotnej.
			 *  \return		Funkcja zwrotna.
			 *
			 * Pobiera funkcję zwrotną wywoływaną przy zmianie wartości zmiennej.
			 *
			 */
			KLSCALLBACK GetCallback(void) const;

			/*! \brief		Ustalenie praw zapisu.
			 *  \param [in]	Active Modyfikator `tylko do odczytu`.
			 *
			 * Ustala czy zmienna powinna zezwalać jedynie na odczyt wartości przy wykonywaniu skryptu.
			 *
			 */
			void SetReadonly(bool Active);

			/*! \brief		Odczytanie praw zapisu.
			 *  \return		Aktualny modyfikator `tylko do odczytu`.
			 *
			 * Sprawdza czy zmienna jest `tylko do odczytu` w skrypcie.
			 *
			 */
			bool IsReadonly(void) const;

			/*! \brief		Zmiana wartość zmiennej.
			 *  \tparam		Data		Typ nowej wartości.
			 *  \param [in]	Value	Nowa wartość.
			 *
			 * Przypisuje do obiektu odpowiednią reprezentacje podanego obiektu po dokonaniu konwersji.
			 *
			 */
			template<typename Data> KLVariable& operator= (const Data& Value);

	};

	protected:

		KLMap<KLVariable, KLString> Variables;	//!< Mapa zmiennych.

	public:

		KLVariables* const Parent;			//!< Zmienne wyższego zakresu.

		/*! \brief		Domyślny konstruktor.
		 *  \param [in]	Scoope System zmiennych wyższego zakresu.
		 *
		 * Tworzy nowy system zmiennych i opcjonalnie zapamiętuje wskaźnik na zmienne z wyższego zakresu.
		 *
		 */
		explicit KLVariables(KLVariables* Scoope = nullptr);

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
		bool Add(const KLString& Name, const KLVariable& Object);

		/*! \brief		Tworzenie zmiennej w systemie.
		 *  \param [in]	Name		Nazwa zmiennej.
		 *  \param [in]	Type		Wyliczenie typu zmiennej.
		 *  \param [in]	Handler	Obiekt funkcyjny wywoływany przy modyfikacji zmiennej.
		 *  \param [in]	Writeable	Ustalenie możliwości edycji zmiennej.
		 *  \return		Powodzenie operacji.
		 *
		 * Tworzy nową zmienną w systemie zgodnie z regułami konstruktora `KLVariable::KLVariable(TYPE, void*)`.
		 *
		 */
		bool Add(const KLString& Name, TYPE Type = NUMBER, KLSCALLBACK Handler = KLSCALLBACK(), bool Writeable = true);

		/*! \brief		Tworzenie zmiennej w systemie.
		 *  \param [in]	Name		Nazwa zmiennej.
		 *  \param [in]	Boolean	Referencja do zbindowanego obiektu.
		 *  \param [in]	Handler	Obiekt funkcyjny wywoływany przy modyfikacji zmiennej.
		 *  \param [in]	Writeable	Ustalenie możliwości edycji zmiennej.
		 *  \return		Powodzenie operacji.
		 *
		 * Tworzy nową zmienną w systemie bindując do niej podany obiekt.
		 *
		 */
		bool Add(const KLString& Name, bool& Boolean, KLSCALLBACK Handler = KLSCALLBACK(), bool Writeable = true);

		/*! \brief		Tworzenie zmiennej w systemie.
		 *  \param [in]	Name		Nazwa zmiennej.
		 *  \param [in]	Number	Referencja do zbindowanego obiektu.
		 *  \param [in]	Handler	Obiekt funkcyjny wywoływany przy modyfikacji zmiennej.
		 *  \param [in]	Writeable	Ustalenie możliwości edycji zmiennej.
		 *  \return		Powodzenie operacji.
		 *
		 * Tworzy nową zmienną w systemie bindując do niej podany obiekt.
		 *
		 */
		bool Add(const KLString& Name, double& Number, KLSCALLBACK Handler = KLSCALLBACK(), bool Writeable = true);

		/*! \brief		Tworzenie zmiennej w systemie.
		 *  \param [in]	Name		Nazwa zmiennej.
		 *  \param [in]	Integer	Referencja do zbindowanego obiektu.
		 *  \param [in]	Handler	Obiekt funkcyjny wywoływany przy modyfikacji zmiennej.
		 *  \param [in]	Writeable	Ustalenie możliwości edycji zmiennej.
		 *  \return		Powodzenie operacji.
		 *
		 * Tworzy nową zmienną w systemie bindując do niej podany obiekt.
		 *
		 */
		bool Add(const KLString& Name, int& Integer, KLSCALLBACK Handler = KLSCALLBACK(), bool Writeable = true);

		/*! \brief		Usuwanie zmiennej z systemu.
		 *  \param [in]	Name Nazwa zmiennej.
		 *  \return		Powodzenie operacji.
		 *
		 * Usuwa wybraną zmienną z systemu zgodnie z regułami `KLVariable::~KLVariable()`.
		 *
		 */
		bool Delete(const KLString& Name);

		/*! \brief		Test obecności zmiennej.
		 *  \param [in]	Name		Nazwa zmiennej.
		 *  \param [in]	Recursive	Wyszukiwanie w kontenerach-rodzicach.
		 *  \return		Obecność zmiennej.
		 *
		 * Sprawdza czy podana zmienna istnieje w systemie.
		 *
		 */
		bool Exists(const KLString& Name, bool Recursive = true) const;

		/*! \brief		Pobranie ilości zmiennych.
		 *  \return		Ilośc zmiennych w obecnym zakresie.
		 *
		 * Sprawdza ile zmiennych istnieje w systemie.
		 *
		 */
		int Size(void) const;

		/*! \brief		Czyszczenie kontenera.
		 *
		 * Usuwa wszystkie zmienne z kontenera.
		 *
		 */
		void Clean(void);

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
		 *  \return		Stała referencja do obiektu zmiennej.
		 *  \warning		Gdy podana zmienna nie istnieje zostają zastosowane reguły `KLMap::operator[]`.
		 *
		 * Wybiera z systemu zmienną o podanej nazwie.
		 *
		 */
		const KLVariable& operator[] (const KLString& Name) const;

		/*! \brief		Operator przypisania.
		 *  \param [in]	Objects Zmienne do skopiowania.
		 *  \return		Referencja do bieżącego obiektu.
		 *
		 * Czyści bieżący system i kopiuje wybrany.
		 *
		 */
		KLVariables& operator = (const KLVariables& Objects);

		KLMap<KLVariable, KLString>::KLMapVarIterator begin(void);
		KLMap<KLVariable, KLString>::KLMapVarIterator end(void);

		KLMap<KLVariable, KLString>::KLMapConstIterator begin(void) const;
		KLMap<KLVariable, KLString>::KLMapConstIterator end(void) const;

};

#endif // KLVARIABLES_HPP
