/***********************************************************************
 *
 * Lightweight Math Parser interpretation for KLLibs
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

#ifndef KLPARSER_HPP
#define KLPARSER_HPP

#ifdef QT_VERSION
	#include "../KLLibs.hpp"
#else
	#define EXPORT
#endif

#include "../containers/klstring.hpp"
#include "../containers/kllist.hpp"

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
class EXPORT KLParser
{

	/*! \brief		Wyliczenie błędu przetwarzania.
	 *
	 * Umożliwia sprawdzenie jaki błąd wystąpił podczas przetwarzania wyrażenia.
	 *
	 */
	public: enum ERROR
	{
		NO_ERROR,				//!< Brak błędu.

		LVALUE_EXPECTED,		//!< Oczekiwano L-wartości.

		UNKNOWN_OPERATOR,		//!< Napotkano nieznany operator.
		UNKNOWN_FUNCTION,		//!< Napotkano nieznaną funkcję.

		EXPECTED_BRACKET,		//!< Oczekiwano na otwierający nawias.

		NOT_ENOUGH_PARAMETERS,	//!< Napotkano zbyt mało parametrów.
		TOO_MANY_PARAMETERS,	//!< Napotkano zbyt wiele parametrów.

		BRACKETS_NOT_EQUAL,		//!< Niepoprawna ilość nawiasów.

		DIVISION_BY_ZERO		//!< Dzielenie przez zero.
	};

	/*! \brief		Klasa bazowa dla tokenu.
	 *
	 * Umozliwia jednolitą obsługę wszystkich tokenów - funkcji, liczb i operatorów.
	 *
	 */
	protected: class KLParserToken
	{

		protected:

			thread_local static ERROR LastError;	//!< Ostatni napotkany błąd.

		public:

			/*! \brief		Konstruktor domyślny.
			 *
			 * Inicjuje wszystkie pola obiektu.
			 *
			 */
			KLParserToken(void);

			/*! \brief		Destruktor.
			 *
			 * Wirtualny destruktor umożliwiający poprawne niszczenie obiektów.
			 *
			 */
			virtual ~KLParserToken(void);

			/*! \brief		Pobranie priorytetu.
			 *  \return		Priorytet tokenu.
			 *  \note			Nie wszystkie tokeny mają znaczący priorytet.
			 *
			 * Zwraca priorytet tokenu w postaci liczby całkowitej.
			 *
			 */
			virtual unsigned GetPriority(void) const;

			/*! \brief		Pobranie ostatniego błędu.
			 *  \return		Token w postaci łańcucha.
			 *
			 * Zwraca wyliczenie ostatniego błędu.
			 *
			 */
			ERROR GetError(void) const;

	};

	/*! \brief		Klasa tokenu będocego liczbą.
	 *
	 * Obsługuje tokeny będące liczbami.
	 *
	 */
	protected: class KLParserValue final : public KLParserToken
	{

		protected:

			double Value;	//!< Wartość zbindowana.

		public:

			/*! \brief		Konstruktor konwertujący z `const char*`.
			 *  \param [in]	Token Wybrany token.
			 *
			 * Konwertuje wybrany napis na liczbę.
			 *
			 */
			KLParserValue(const char* Token);

			/*! \brief		Konstruktor konwertujący z `double`.
			 *  \param [in]	Token Wybrany token.
			 *
			 * Konwertuje wybrany napis na liczbę.
			 *
			 */
			KLParserValue(double Token);

			/*! \brief		Destruktor.
			 *
			 * Wirtualny destruktor umożliwiający poprawne niszczenie obiektów.
			 *
			 */
			virtual ~KLParserValue(void) override;

			/*! \brief		Pobranie wartości.
			 *  \return		Przechowywana wartość.
			 *
			 * Zwraca wartość liczby którą reprezentuje token.
			 *
			 */
			double GetValue(void) const;

	};

	/*! \brief		Klasa tokenu będocego operatorem.
	 *
	 * Obsługuje tokeny będące operatorami.
	 *
	 */
	protected: class KLParserOperator final : public KLParserToken
	{

		/*! \brief		Wyliczenie znaczenia operatora.
		 *
		 * Pozwala ograniczyć operatory do wybranego zakresu i przyspiesza wykonywanie działań.
		 *
		 */
		public: enum OPERATOR
		{
			UNKNOWN,		//!< Nieznany operator.

			ADD,			//!< Dodawanie: `a + b`.
			SUB,			//!< Odejmowanie: `a - b`.
			MUL,			//!< Mnożenie: `a * b`.
			DIV,			//!< Dzielenie: `a / b`.
			MOD,			//!< Reszta z dzielenia: `a % b`.
			POW,			//!< Potęgowanie: `a^b`.

			EQ,			//!< Jest równy: `a == b`.
			NEQ,			//!< Jest różny: `a != b`.
			GT,			//!< Jest większy: `a > b`.
			LT,			//!< Jest mniejszy: `a < b`.
			GE,			//!< Jest większy lub równy: `a >= b`.
			LE,			//!< Jest mniejszy lub równy: `a <= b`.

			AND,			//!< Iloczyn logiczny: `a && b`.
			OR,			//!< Suma logiczna: `a || b`.

			L_BRACKET,	//!< Nawias otwierający: `(`.
			R_BRACKET		//!< Nawias zamykający: `)`.
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

		protected:

			static const KLParserOperatorData Operators[];	//!< Tabela dostępnych operatorów.

			OPERATOR Operator;							//!< Symbol operatora.

		public:

			/*! \brief		Konstruktor konwertujący z `const char*`.
			 *  \param [in]	Token Wybrany token.
			 *
			 * Konwertuje wybrany napis na operator.
			 *
			 */
			KLParserOperator(const char* Token);

			/*! \brief		Konstruktor konwertujący z wyliczenia `OPERATOR`.
			 *  \param [in]	Token Wybrany token.
			 *
			 * Tworzy operator o wybranym symbolu.
			 *
			 */
			KLParserOperator(OPERATOR Token);

			/*! \brief		Destruktor.
			 *
			 * Wirtualny destruktor umożliwiający poprawne niszczenie obiektów.
			 *
			 */
			virtual ~KLParserOperator(void) override;

			/*! \brief		Obliczenie wyniku działania.
			 *  \param [in,out]	Values Lista dostępnych parametrów.
			 *  \return		Wynik wyrażenia.
			 *  \see			GetError().
			 *
			 * Pobiera ze stosu wymaganą liczbę parametrów i na podstawie danych operatora wylicza odpowiednią wartość wyrażenia. W przypadku niepowodzenia zapamiętuje ostatni błąd.
			 *
			 */
			double GetValue(KLList<double>& Values) const;

			/*! \brief		Pobranie numeru operatora.
			 *  \return		Wynik wyrażenia.
			 *
			 * Zwraca identyfikator bierzącego operatora.
			 *
			 */
			OPERATOR GetOperator(void) const;

			virtual unsigned GetPriority(void) const override;

	};

	/*! \brief		Klasa tokenu będocego funkcją.
	 *
	 * Obsługuje tokeny będące funkcjami.
	 *
	 */
	protected: class KLParserFunction final : public KLParserToken
	{

		/*! \brief		Wyliczenie znaczenia funkcji.
		 *
		 * Pozwala ograniczyć funkcje do wybranego zakresu i przyspiesza wykonywanie działań.
		 *
		 */
		public: enum FUNCTION
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

			NOT		//!< Negacja logiczna: `not(x)`.
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

		protected:

			static const KLParserFunctionData Functions[];	//!< Tablica znanych funkcji.

			FUNCTION Function;							//!< Indeks funkcji.

		public:

			/*! \brief		Konstruktor konwertujący z `const char*`.
			 *  \param [in]	Token Wybrany token.
			 *
			 * Konwertuje wybrany napis na operator.
			 *
			 */
			KLParserFunction(const char* Token);

			/*! \brief		Konstruktor konwertujący z wyliczenia `FUNCTION`.
			 *  \param [in]	Token Wybrany token.
			 *
			 * Tworzy funkcję na podstawie podanego wyliczenia.
			 *
			 */
			KLParserFunction(FUNCTION Token);

			/*! \brief		Destruktor.
			 *
			 * Wirtualny destruktor umożliwiający poprawne niszczenie obiektów.
			 *
			 */
			virtual ~KLParserFunction(void) override;

			/*! \brief		Obliczenie wyniku działania.
			 *  \param [in,out]	Values Lista dostępnych parametrów.
			 *  \return		Wynik wyrażenia.
			 *  \see			GetError().
			 *
			 * Pobiera ze stosu wymaganą liczbę parametrów i na podstawie danych funkcji wylicza odpowiednią wartość wyrażenia. W przypadku niepowodzenia zapamiętuje ostatni błąd.
			 *
			 */
			double GetValue(KLList<double>& Values) const;

			/*! \brief		Pobranie numeru funkcji.
			 *  \return		Wynik wyrażenia.
			 *
			 * Zwraca identyfikator bierzącej funkcji.
			 *
			 */
			FUNCTION GetFunction(void) const;

	};

	protected:

		/*! \brief		Przekształcenie wyrażenia do notacli RPN.
		 *  \param [out]	Tokens	Wyjściowa lista tokenów.
		 *  \param [in]	Code		Wyrażenie do przetworzenia.
		 *  \return		Powodzenie operacji.
		 *
		 * Parsuje wyrażenie i zamienia je na postać Odwrotnej Notacji Polskiej.
		 *
		 */
		bool GetTokens(KLList<KLParserToken*>& Tokens,
					const KLString& Code);

		double LastValue;				//!< Ostatnia poprawnie obliczona wartość wyrażenia.

		ERROR LastError;				//!< Ostatni odnotowany błąd.

	public:

		/*! \brief		Domyślny konstruktor.
		 *  \param [in]	Code Wyrażenie do obliczenia.
		 *  \return 		Powodzenie operacji.
		 *  \see			LastError(), LastValue().
		 *
		 * Przetwarza podane wyrażenie i zwraca powodzenie jego wykonania.
		 *
		 */
		bool Evaluate(const KLString& Code);

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
