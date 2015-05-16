/***********************************************************************
 *
 * {description}
 * Copyright (C) {year}  {fullname}
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

#ifndef KLSCRIPT_HPP
#define KLSCRIPT_HPP

#ifdef QT_VERSION
	#include "kllibs.hpp"
#else
	#define EXPORT
#endif

#include "containers/klstring.hpp"

#include "script/klvariables.hpp"
#include "script/klbindings.hpp"
#include "script/klparser.hpp"

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
 *  \warning	Nie zawsze sprawdzanie poprawności zapisu i składni odbywa się dokładnie tak, jak powinno. W przypadku złamania reguł składni może okazać się, że wychwycony zostanie inny błąd, lub z jakiś przyczyn parser uzna wyrażenie za poprawne. Należy dbać zatem by wyrażenia były poprawne.
 *
 * Prosta i lekka interpretacja języka skryptowego podobnego częściowo do basha. Do przetwarzania wyrażeń używa parsera `KLParser`, obsługuje bindowania zmiennych `KLVariables`, bindowanie funkcji `KLBindings`, tworzenie zmiennych i strukturę `if else`.
 *
 * Utworzone zmienne są niszczone po zakończeniu skryptu. Do funkcji przekazywane są zmienne ze stosu w osobnym zakresie, wraz z dołączeniem zasięgu całego skryptu.
 *
 * Po każdym wyrażeniu (instrukcji) musi zostać umieszczony terminator `;`. Dotyczy to także konstrukcji `if`, `else`, `endif`, `while`, `done` itd. Taka restrykcja wpraszcza parser do minimalnego stopnia skomplikowania.
 *
 */
class EXPORT KLScript
{

	/*! \brief		Wyliczenie operacji.
	 *
	 * Umożliwia sprawdzenie jaką operację należy przetworzyć.
	 *
	 */
	protected: enum OPERATION
	{
		UNKNOWN,	//!< Nieznana operacja.

		COMMENT,	//!< Komentarz: `# tekst`.

		VAR,		//!< Utworzenie zmiennej: `var zmA, zmB, ...`.

		SET,		//!< Przypisanie do zmiennej: `set ZM wyrażenie`.
		CALL,	//!< Wywołanie zbindowanej funkcji: `call FN paramA, paramB, ...`.

		T_IF,	//!< Konstrukcja warunkowa: `if wyrażenie`.
		T_ELSE,	//!< Konstrukcja warunkowa: `else`.
		T_ENDIF,	//!< Konstrukcja warunkowa: `fi`.

		T_WHILE,	//!< Konstrukcja warunkowa: `while`.
		T_DONE,	//!< Konstrukcja warunkowa: `done`.

		T_EXIT,	//!< Zakończenie skryptu: `exit`.

		CODE_END	//!< Koniec skryptu: znak `'\0'`.
	};

	/*! \brief		Wyliczenie błędu.
	 *
	 * Umożliwia sprawdzenie jaki błąd przerwał działanie skryptu.
	 *
	 */
	public: enum ERROR
	{
		NO_ERROR,			//!< Brak błędu.

		UNDEFINED_VARIABLE,	//!< Niezdefiniowana zmienna.
		UNDEFINED_FUNCTION,	//!< Niezdefiniowana funkcja.

		MISSING_PARAMETERS,	//!< Brakujące parametry.

		EXPECTED_ENDIF_TOK,	//!< Brak oczekiwanego `endif`.
		EXPECTED_DONE_TOK,	//!< Brak oczekiwanego `done`.

		UNKNOWN_EXPRESSION,	//!< Nieznane wyrażenie.

		WRONG_EVALUATION	//!< Błąd w wyrażeniu matematycznym.
	};

	protected:

		/*! \brief		Pobranie numeru operacji.
		 *  \param [in]	Code Przetwarzany kod.
		 *  \return		Numer wyrażenia.
		 *
		 * Pobiera numer wyrażenia do przetworzenia.
		 *
		 */
		OPERATION GetToken(const KLString& Code);

		/*! \brief		Pobranie parametru operacji.
		 *  \param [in]	Code Przetwarzany kod.
		 *  \return		Parametr wyrażenia.
		 *
		 * Pobiera parametr przetwarzanego wyrażenia.
		 *
		 */
		KLString GetParam(const KLString& Code);

		/*! \brief		Pobranie wartości liczbowej.
		 *  \param [in]	Code Przetwarzany kod.
		 *  \return		Wartośc wyrażenia.
		 *
		 * Oblicza wartość kolejnego napotkanego wyrażenia matematycznego.
		 *
		 */
		bool GetValue(const KLString& Code);

		ERROR LastError;	//!< Wyliczenie ostatniego błędu.

		int Process;		//!< Aktualny krok przetwarzania.

	public:

		KLVariables	Variables;	//!< Zmienne i ich bindy.

		KLBindings	Bindings;		//!< Bindy lokalnych funkcji.

		KLParser		Parser;		//!< Parser matematyczny.

		/*! \brief		Domyślny konstruktor.
		 *
		 * Inicjuje obiekt i tworzy specjalną zmienną `return` do przechowywania wyników funkcji.
		 *
		 */
		KLScript(void);

		/*! \brief		Wykonanie kodu.
		 *  \param [in]	Code do wykonania.
		 *  \return		Powodzenie operacji.
		 *
		 * Przetwarza wybrany kod i zwraca powodzenie operacji.
		 *
		 */
		bool Evaluate(const KLString& Code);

		/*! \brief		Pobranie ostatniego błędu.
		 *  \return		Ostatni błąd.
		 *
		 * Zwraca ostatni napotkany błąd.
		 *
		 */
		ERROR GetError(void) const;

};

#endif // KLSCRIPT_HPP
