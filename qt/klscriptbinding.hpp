/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  QT bindings for KLLib KLScript                                         *
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

#ifndef KLSCRIPTBINDING_HPP
#define KLSCRIPTBINDING_HPP

#include "../libbuild.hpp"

#include "../script/klscript.hpp"

#include <QObject>
#include <QRegExp>

/*! \file		klscriptbinding.hpp
 *  \brief	Deklaracje dla klasy KLScriptbinding i jej składników.
 *
 */

/*! \file		klscriptbinding.cpp
 *  \brief	Implementacja klasy KLScriptbinding i jej składników.
 *
 */

/*! \brief	Bindy QT do `KLScript`.
 *
 * Bazujący na `QObject` i `KLScript` bind ułatwiający współpracę z `KLScript` w środowisku QT.
 *
 */
class KLLIBS_EXPORT KLScriptbinding : public QObject, KLScript
{

		Q_OBJECT

	protected:

		QString LastCode;	//!< Ostatni przetwarzany skrypt.

		int LastLine;		//!< Ostatnia przetwarzana linia.

	public:

		using KLScript::Functions;
		using KLScript::Variables;
		using KLScript::Bindings;
		using KLScript::Parser;

		/*! \brief		Konstruktor.
		 *  \param [in]	Parent	Rodzic obiektu.
		 *  \param [in]	Scoope	Wyższy poziom dla zmiennych skryptu.
		 *
		 * Buduje obiekt zgodnie z mechanizmem `QObject`.
		 *
		 */
		explicit KLScriptbinding(KLVariables* Scoope = nullptr,
							QObject* Parent = nullptr);

		/*! \brief		Destruktor.
		 *
		 * Niszczy obiekt zgodnie z mechanizmem `QObject`.
		 *
		 */
		virtual ~KLScriptbinding(void) override;

		/*! \brief		Zapisanie skryptu.
		 *  \param [in]	Script Skrypt do zapisania.
		 *
		 * Zapamiętuje skrypt w celu późniejszego wywołania.
		 *
		 */
		void SetCode(const QString& Script);

		/*! \brief		Pobranie skryptu.
		 *  \return		Bieżący skrypt.
		 *
		 * Zwraca aktualnie przechowywany skrypt.
		 *
		 */
		QString GetCode(void) const;

		/*! \brief		Wywołanie skryptu.
		 *  \return 		Powodzenie operacji.
		 *  \see			LastError(), LastValue().
		 *
		 * Przetwarza podane wyrażenie i zwraca powodzenie jego wykonania.
		 *
		 */
		bool Evaluate(void);

		/*! \brief		Sprawdzenie skryptu.
		 *  \param [in]	Script Skrypt do sprawdzenia.
		 *  \return 		Powodzenie operacji.
		 *
		 * Przetwarza podany skrypt i sprawdza jego poprawność.
		 *
		 */
		bool Validate(const QString& Script);

		/*! \brief		Optymalizacja skryptu.
		 *
		 * Usuwa ze skryptu komentarze i niepotrzebne białe znaki.
		 *
		 */
		void Optimize(void);

		/*! \brief		Pobranie wartości.
		 *  \return		Ostatnia poprawnie obliczona wartość.
		 *  \see			Evaluate().
		 *
		 * Pobiera ostatnią obliczoną wartość.
		 *
		 */
		ERROR GetError(void) const;

		/*! \brief		Pobranie opisu ostatniego błędu.
		 *  \return		Opis ostatniego błędu.
		 *  \see			GetError().
		 *
		 * Zwraca czytelny dla człowieka opis napotkanego błędu.
		 *
		 */
		QString GetMessage(void) const;

		/*! \brief		Pobranie numeru linii.
		 *  \return		Ostatnio przetwarzana linia.
		 *
		 * Pobiera ostatnio przetwarzaną linie.
		 *
		 */
		int GetLine(void) const;

		/*! \brief		Pobranie wartości.
		 *  \return		Ostatnia poprawnie obliczona wartość.
		 *  \see			Evaluate().
		 *
		 * Pobiera ostatnią obliczoną wartość.
		 *
		 */
		double GetReturn(void) const;

		/*! \brief		Optymalizacja skryptu.
		 *  \param [in]	Script Skrypt do optymalizacji.
		 *  \return		Zoptymalizowany skrypt.
		 *
		 * Usuwa ze skryptu komentarze i niepotrzebne białe znaki.
		 *
		 */
		static QString Optimize(const QString& Script);

		/*! \brief		Tłumaczenie kodu błędu.
		 *  \param [in]	Code Kod błędu.
		 *  \return		Czytelny opis błędu.
		 *
		 * Tłumaczy podany numer błędu na czytelny opis.
		 *
		 */
		static QString Errorcode(ERROR Code);

		using KLScript::Terminate;

	signals:

		/*! \brief		Zdarzenie obliczenia wartości.
		 *  \param [in]	Value	Zwrócona wartość.
		 *  \param [in]	Error	Zwrócony kod błędu.
		 *  \see			Evaluate().
		 *
		 * Zdarzenie wywoływane przy poprawnym obliczeniu wartości wyrażenia.
		 *
		 */
		void onEvaluate(double Value, ERROR Error);

};

#endif // KLSCRIPTBINDING_HPP
