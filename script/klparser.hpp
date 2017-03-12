/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Lightweight Math Parser interpretation for KLLibs                      *
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

#ifndef KLPARSER_HPP
#define KLPARSER_HPP

#include "../libbuild.hpp"

#include "../containers/klstring.hpp"
#include "../containers/kllist.hpp"
#include "../script/klvariables.hpp"

#include <ctype.h>
#include <math.h>

/*! \file		klparser.hpp
 *  \brief	Deklaracje dla klasy KLParser i jej składników.
 *
 */

/*! \file		klparser.cpp
 *  \brief	Implementacja klasy KLParser i jej składników.
 *
 */

/*! \brief	Lekka interpretacja parsera matematycznego.
 *
 * Prosta i lekka interpretacja matematycznego parsera. Wspiera wbudowane funkcję z biblioteki `math.c`, obsługuję kolejność działań i priorytety operatorów. Umożliwia działania logiczne i matematyczne.
 *
 */
class KLLIBS_EXPORT KLParser
{

	/*! \brief		Wyliczenie błędu przetwarzania.
	 *
	 * Umożliwia sprawdzenie jaki błąd wystąpił podczas przetwarzania wyrażenia.
	 *
	 */
	public: enum ERROR
	{
		NO_ERROR,				//!< Brak błędu.

		UNEXPECTED_OPERATOR,	//!< Nieoczekiwany operator.

		UNKNOWN_OPERATOR,		//!< Napotkano nieznany operator.
		UNKNOWN_EXPRESSION,		//!< Napotkano nieznane wyrażenie.

		EXPECTED_BRACKET,		//!< Oczekiwano na otwierający lub zamykający nawias.

		NOT_ENOUGH_PARAMETERS,	//!< Napotkano zbyt mało parametrów.
		TOO_MANY_PARAMETERS,	//!< Napotkano zbyt wiele parametrów.

		BRACKETS_NOT_EQUAL		//!< Niepoprawna ilość nawiasów.
	};

	/*! \brief		Klasa bazowa dla tokenu.
	 *
	 * Umozliwia jednolitą obsługę wszystkich tokenów - funkcji, liczb i operatorów.
	 *
	 */
	protected: class KLParserToken
	{

		/*! \brief		Wyliczenie typu tokenu.
		 *
		 * Określa z jaki typ ma bieżący token. Rozwiązanie to pozwala uniknąć stosowania polimorfizmu i operatora dynamic_cast.
		 *
		 */
		public: enum class CLASS
		{
			VALUE,	//!< Wartość liczbowa.
			OPERATOR,	//!< Operator.
			FUNCTION	//!< Funkcja.
		};

		/*! \brief		Wyliczenie znaczenia operatora.
		 *
		 * Pozwala ograniczyć operatory do wybranego zakresu i przyspiesza wykonywanie działań.
		 *
		 */
		public: enum class OPERATOR
		{
			UNKNOWN,		//!< Nieznany operator.

			ROUND,		//!< Operacja zaokrąglenia `a ~ b`.
			ADD,			//!< Dodawanie: `a + b`.
			SUB,			//!< Odejmowanie: `a - b`.
			MUL,			//!< Mnożenie: `a * b`.
			DIV,			//!< Dzielenie: `a / b`.
			MOD,			//!< Reszta z dzielenia: `a % b`.
			POW,			//!< Potęgowanie: `a^b`.

			EQ,			//!< Jest równy: `a = b`.
			NEQ,			//!< Jest różny: `a <> b`.
			GT,			//!< Jest większy: `a > b`.
			LT,			//!< Jest mniejszy: `a < b`.
			GE,			//!< Jest większy lub równy: `a >= b`.
			LE,			//!< Jest mniejszy lub równy: `a <= b`.

			OR,			//!< Suma logiczna: `a | b`.
			FOR,			//!< Suma rozmyta: `a ? b`.
			AND,			//!< Iloczyn logiczny: `a & b`.
			FAND,		//!< Iloczyn rozmyty: `a @ b`.

			L_BRACKET,	//!< Nawias otwierający: `(`.
			R_BRACKET		//!< Nawias zamykający: `)`.
		};

		/*! \brief		Wyliczenie znaczenia funkcji.
		 *
		 * Pozwala ograniczyć funkcje do wybranego zakresu i przyspiesza wykonywanie działań.
		 *
		 */
		public: enum class FUNCTION
		{
			UNKNOWN,	//!< Nieznana funkcja.

			SIN,		//!< Sunus: `sin(x)`.
			COS,		//!< Cosinus: `cos(x)`.
			TAN,		//!< Tangens: `tan(x)`.

			ABS,		//!< Wartość bezwzględna: `abs(x)`.

			EXP,		//!< Liczba `e` do potęgi `x`: `exp(x)`.
			SQRT,	//!< Pierwiastek: `sqrt(x)`.
			LOG,		//!< Logarytm dziesiętny: `log(x)`.
			LN,		//!< Logarytm naturalny: `ln(x)`.

			NOT,		//!< Negacja logiczna: `!x`.

			MINUS	//!< Przeciwieństwo liczby: `-x`.
		};

		/*! \brief		Struktura danych operatora.
		 *  \warning		Numer operatora musi być zgodny z pozycją operatora w tabeli.
		 *
		 * Umożliwia utworzenie listy dostępnych operatorów wraz z ich danymi.
		 *
		 */
		public: struct KLParserOperatorData
		{
			OPERATOR Operator;	//!< Numer operatora.

			const char* Token;	//!< Symbol operatora.

			unsigned Priority;	//!< Priorytet operatora.
		};

		/*! \brief		Struktura danych operatora.
		 *  \warning		Numer operatora musi być zgodny z pozycją operatora w tabeli.
		 *
		 * Umożliwia utworzenie listy dostępnych operatorów wraz z ich danymi.
		 *
		 */
		public: struct KLParserFunctionData
		{
			FUNCTION Function;	//!< Indeks funkcji.

			const char* Token;	//!< Nazwa funkcji.
		};

		/*! \brief		Unia danych tokenu.
		 *
		 * Zawiera wybrane informacje o danym tokenie.
		 *
		 */
		public: union TOKEN
		{
			double Value;		//!< Wartość liczbowa (o ile token jest liczbą).

			OPERATOR Operator;	//!< ID operatora (o ile token jest operatorem).

			FUNCTION Function;	//!< ID funkcji (o ile token jest funkcją).
		};

		protected:

			static const KLParserOperatorData Operators[];	//!< Tabela dostępnych operatorów.
			static const KLParserFunctionData Functions[];	//!< Tablica znanych funkcji.

			thread_local static ERROR LastError;			//!< Ostatni napotkany błąd.

			TOKEN Data;								//!< Dane tokenu.

		public:

			const CLASS Class;							//!< Klasa tokenu.

			/*! \brief		Konstruktor domyślny.
			 *  \param [in]	Token		Token w formie łańcucha znaków.
			 *  \param [in]	TokenClass	Klasa tokenu.
			 *
			 * Na podstawie podanego typu tokena wybiera odpowiednie informacje z tabeli.
			 *
			 */
			KLParserToken(const char* Token, CLASS TokenClass);

			/*! \brief		Konstruktor konwertujący z `double`.
			 *  \param [in]	Value Wartość liczbowa.
			 *
			 *Tworzy token na podstawie wartości liczbowej.
			 *
			 */
			KLParserToken(double Value);

			/*! \brief		Konstruktor konwertujący z `OPERATOR`.
			 *  \param [in]	Operator ID operatora.
			 *
			 *Tworzy token na podstawie ID operatora.
			 *
			 */
			KLParserToken(OPERATOR Operator);

			/*! \brief		Konstruktor konwertujący z `FUNCTION`.
			 *  \param [in]	Function ID funkcji.
			 *
			 *Tworzy token na podstawie ID funkcji.
			 *
			 */
			KLParserToken(FUNCTION Function);

			/*! \brief		Pobranie priorytetu.
			 *  \return		Priorytet tokenu.
			 *  \note			Nie wszystkie tokeny mają znaczący priorytet.
			 *
			 * Zwraca priorytet tokenu w postaci liczby całkowitej.
			 *
			 */
			unsigned GetPriority(void) const;

			/*! \brief		Pobranie priorytetu.
			 *  \param [in]	Values Stos wartości parametrów.
			 *  \return		Wartość obliczeń lub liczby.
			 *
			 * Zwraca wartość liczbową obliczoną na podstawie tokenu i pobranych ze stosu parametrów.
			 *
			 */
			double GetValue(KLList<double>* Values = nullptr) const;

			/*! \brief		Pobranie ID operatora.
			 *  \return		ID operatora.
			 *
			 * Zwraca ID operatora jeśli token jest operatorem.
			 *
			 */
			OPERATOR GetOperator(void) const;

			/*! \brief		Pobranie ID funkcji.
			 *  \return		ID funkcji.
			 *
			 * Zwraca ID funkcji jeśli token jest funkcją.
			 *
			 */
			FUNCTION GetFunction(void) const;

			/*! \brief		Pobranie ostatniego błędu.
			 *  \return		Token w postaci łańcucha.
			 *
			 * Zwraca wyliczenie ostatniego błędu.
			 *
			 */
			ERROR GetError(void) const;

	};

	protected:

		/*! \brief		Przekształcenie wyrażenia do notacli RPN.
		 *  \param [out]	Tokens	Wyjściowa lista tokenów.
		 *  \param [in]	Code		Wyrażenie do przetworzenia.
		 *  \param [in]	Scoope	Zasięg zmiennych.
		 *  \return		Powodzenie operacji.
		 *
		 * Parsuje wyrażenie i zamienia je na postać Odwrotnej Notacji Polskiej.
		 *
		 */
		bool GetTokens(KLList<KLParserToken*>& Tokens, const KLString& Code, const KLVariables* Scoope, const double Return);

		double LastValue;				//!< Ostatnia poprawnie obliczona wartość wyrażenia.

		ERROR LastError;				//!< Ostatni odnotowany błąd.

	public:

		/*! \brief		Wywołanie skryptu.
		 *  \param [in]	Code		Wyrażenie do obliczenia.
		 *  \param [in]	Scoope	Zasięg zmiennych.
		 *  \return 		Powodzenie operacji.
		 *  \see			GetError(), GetValue().
		 *
		 * Przetwarza podane wyrażenie i zwraca powodzenie jego wykonania.
		 *
		 */
		bool Evaluate(const KLString& Code, const KLVariables* Scoope = nullptr, const double Return = NAN);

		/*! \brief		Pobranie wartości.
		 *  \return		Ostatnia poprawnie obliczona wartość.
		 *  \see			Evaluate(const KLString&).
		 *
		 * Pobiera ostatnią obliczoną wartość.
		 *
		 */
		double GetValue(void) const;

		/*! \brief		Pobranie błędu.
		 *  \return		Ostatni napotkany błąd.
		 *  \see			Evaluate(const KLString&).
		 *
		 * Pobiera ostatni błąd znaleziony w wyrażeniu.
		 *
		 */
		ERROR GetError(void) const;

};

#endif // KLPARSER_HPP
