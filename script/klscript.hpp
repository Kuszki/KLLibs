/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Lightweight Script Engine interpretation for KLLibs                    *
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

#ifndef KLSCRIPT_HPP
#define KLSCRIPT_HPP

#define BIND(function) 		#function, function								//!< Makro ułatwiające bindowanie funkcji pod taką samą nazwą, jak w kodzie.

#include "../libbuild.hpp"

#include "../containers/klstring.hpp"

#include "klvariables.hpp"
#include "klbindings.hpp"
#include "klparser.hpp"

#include <ctype.h>

/*! \file		klscript.hpp
 *  \brief	Deklaracje dla klasy KLScript i jej składników.
 *
 */

/*! \file		klscript.cpp
 *  \brief	Implementacja klasy KLScript i jej składników.
 *
 */

/*! \brief	Lekka interpretacja języka skryptowego.
 *  \warning	Nie zawsze sprawdzanie poprawności zapisu i składni odbywa się dokładnie tak, jak powinno. W przypadku złamania reguł składni może okazać się, że wychwycony zostanie inny błąd, lub z jakiś przyczyn parser uzna wyrażenie za poprawne. Należy zatem dbać by wyrażenia były poprawne.
 *
 * Prosta i lekka interpretacja języka skryptowego podobnego częściowo do basha. Do przetwarzania wyrażeń używa parsera `KLParser`, obsługuje bindowania zmiennych `KLVariables`, bindowanie funkcji `KLBindings`, tworzenie zmiennych i strukturę `if else`.
 *
 * Utworzone zmienne są niszczone po zakończeniu skryptu. Do funkcji przekazywane są zmienne ze stosu w osobnym zakresie, wraz z dołączeniem zasięgu całego skryptu.
 *
 * Po każdym wyrażeniu (instrukcji) musi zostać umieszczony terminator `;`. Dotyczy to także konstrukcji `if`, `else`, `endif`, `while`, `done` itd. Taka restrykcja upraszcza parser do minimalnego stopnia skomplikowania.
 *
 */
class KLLIBS_EXPORT KLScript
{

	/*! \brief		Wyliczenie operacji.
	 *
	 * Umożliwia sprawdzenie jaką operację należy przetworzyć.
	 *
	 */
	protected: enum OPERATION
	{
		UNKNOWN,	//!< Nieznana operacja.
		EMPTY,	//!< Pusta operacja.
		END,		//!< Naturalny koniec skryptu.

		SET,		//!< Przypisanie do zmiennej: `set ZM wyrażenie`.
		CALL,	//!< Wywołanie zbindowanej funkcji: `call FN paramA, paramB, ...`.
		GOTO,	//!< Wywołanie zdefiniowanej funkcji: `goto FN paramA, paramB, ...`.

		VAR,		//!< Utworzenie zmiennej: `var zmA, zmB, ...`.
		EXP,		//!< Eksportuje zmienną: `exp zmA, zmB, ...`.
		POP,		//!< Ściąga wartość do zmiennej ze stosu `pop zmA, zmB, ...`.

		T_IF,	//!< Konstrukcja warunkowa: `if wyrażenie`.
		T_ELSE,	//!< Konstrukcja warunkowa: `else`.
		T_ENDIF,	//!< Konstrukcja warunkowa: `fi`.

		T_WHILE,	//!< Konstrukcja warunkowa: `while`.
		T_DONE,	//!< Konstrukcja warunkowa: `done`.

		T_DEF,	//!< Definicja funkcji: `define funkcja`.
		T_END,	//!< Zakończenie funkcji: `end`.

		T_RETURN,	//!< Zakończenie skryptu i zwrócenie wartości: `return wyrażenie`.

		EXIT,	//!< Zakończenie skryptu: `exit`.
	};

	/*! \brief		Wyliczenie błędu.
	 *
	 * Umożliwia sprawdzenie jaki błąd przerwał działanie skryptu.
	 *
	 */
	public: enum ERROR
	{
		NO_ERROR,				//!< Brak błędu.

		UNDEFINED_VARIABLE,		//!< Niezdefiniowana zmienna.
		UNDEFINED_FUNCTION,		//!< Niezdefiniowana funkcja.

		EXPECTED_ENDIF_TOK,		//!< Brak oczekiwanego `fi`.
		EXPECTED_DONE_TOK,		//!< Brak oczekiwanego `done`.
		EXPECTED_TERMINATOR,	//!< Oczekiwano terminatora `;`.

		EMPTY_FUNCTION,		//!< Pusta funkcja.
		EMPTY_EXPRESSION,		//!< Puste wyrażenie (np `;;`).
		UNKNOWN_EXPRESSION,		//!< Nieznane wyrażenie.

		WRONG_SCRIPTCODE,		//!< Niepoprawny lub pusty skrypt.
		WRONG_PARAMETERS,		//!< Brakujące lub błędne parametry.
		WRONG_EVALUATION,		//!< Błąd w wyrażeniu matematycznym.

		VARIABLE_READONLY,		//!< Zmienna tylko do odczytu.

		SCRIPT_TERMINATED		//!< Użytkownik przerwał skrypt.
	};

	protected:

		/*! \brief		Pobranie numeru operacji.
		 *  \param [in]	Script Przetwarzany kod.
		 *  \return		Numer wyrażenia.
		 *
		 * Pobiera numer wyrażenia do przetworzenia.
		 *
		 */
		OPERATION GetToken(const KLString& Script);

		/*! \brief		Pobranie parametru operacji.
		 *  \param [in]	Script Przetwarzany kod.
		 *  \return		Parametr wyrażenia.
		 *
		 * Pobiera parametr przetwarzanego wyrażenia.
		 *
		 */
		KLString GetParam(const KLString& Script);

		/*! \brief		Pobranie nazwy obiektu.
		 *  \param [in]	Script Przetwarzany kod.
		 *  \return		Parametr wyrażenia.
		 *
		 * Pobiera nazwę zmiennej lub innego obiektu.
		 *
		 */
		KLString GetName(const KLString& Script);

		/*! \brief		Pobranie wartości liczbowej.
		 *  \param [in]	Script	Przetwarzany kod.
		 *  \param [in]	Scoope	Zakres zmiennych.
		 *  \return		Wartośc wyrażenia.
		 *
		 * Oblicza wartość kolejnego napotkanego wyrażenia matematycznego.
		 *
		 */
		bool GetValue(const KLString& Script, KLVariables& Scoope);

		/*! \brief		Pominięcie komentarza.
		 *  \param [in]	Script Przetwarzany kod.
		 *  \return		Bierzący punkt w skrypcie.
		 *
		 * Ignoruje treść komantarza aż do napotkania nowej linii.
		 *
		 */
		int SkipComment(const KLString& Script);

		volatile bool Sigterm;				//!< Sygnał zakończenia skryptu.

		double LastReturn;					//!< Ostatnia zwrócona wartość.

		int LastProcess;					//!< Aktualny krok przetwarzania.

		ERROR LastError;					//!< Wyliczenie ostatniego błędu.

	public:

		KLMap<KLString, KLString> Functions;	//!< Funkcje zdefiniowane za pomocą skryptu.

		KLVariables	Variables;			//!< Zmienne i ich bindy.

		KLBindings	Bindings;				//!< Bindy lokalnych funkcji.

		KLParser		Parser;				//!< Parser matematyczny.

		/*! \brief		Domyślny konstruktor.
		 *  \param [in]	Scoope Wyższy poziom dla zmiennych skryptu.
		 *
		 * Inicjuje obiekt i tworzy specjalną zmienną `return` do przechowywania wyników funkcji.
		 *
		 */
		KLScript(KLVariables* Scoope = nullptr);

		/*! \brief		Wykonanie kodu.
		 *  \param [in]	Script	Skrypt do przetworzenia.
		 *  \param [in]	Params	Stos ze zmiennymi do pobrania.
		 *  \return		Powodzenie operacji.
		 *
		 * Przetwarza wybrany kod i zwraca powodzenie operacji.
		 *
		 */
		bool Evaluate(const KLString& Script, KLList<double>* Params = nullptr);

		/*! \brief		Sprawdzenie kodu.
		 *  \param [in]	Script	Skrypt do przetworzenia.
		 *  \param [in]	Scoope	Globalny zakres zmiennych.
		 *  \return		Powodzenie operacji.
		 *
		 * Przetwarza wybrany kod pod kątem błędów składni i zwraca powodzenie operacji.
		 *
		 */
		bool Validate(const KLString& Script, KLVariables* Scoope = nullptr);

		/*! \brief		Przerwanie skryptu.
		 *
		 * Ustala zmienną odpowiedzialną za zakończenie skryptu przy następnej iteracji. Metode należy wywołać za pośrednictwem innego wątku lub w kodzie przerwanai watchdoga.
		 *
		 */
		void Terminate(void);

		/*! \brief		Pobranie ostatniego błędu.
		 *  \return		Ostatni błąd.
		 *  \see			Evaluate(const KLString&), Validate(const KLString&).
		 *
		 * Zwraca ostatni napotkany błąd.
		 *
		 */
		ERROR GetError(void) const;

		/*! \brief		Pobranie ostatnio zwróconej wartości.
		 *  \return		Ostatna zwrócona wartość.
		 *  \see			Evaluate(const KLString&), Validate(const KLString&).
		 *
		 * Zwraca ostatnią wartość przekazaną do wyrażenia `return`.
		 *
		 */
		double GetReturn(void) const;

		/*! \brief		Obliczenie numeru linii ostatniego błędu.
		 *  \param [in]	Script Ostatni skrypt.
		 *  \return		Numer linii ostatniego błędu.
		 *
		 * Oblicza i zwraca numer linii w której ostatnio wystąpił błąd. Wymaga podania ostatnio przetwarzanego skryptu.
		 *
		 */
		int GetLine(const KLString& Script) const;

};

#endif // KLSCRIPT_HPP
