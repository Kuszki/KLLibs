/***********************************************************************
 *
 *  QT bindings for KLLib KLScript
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

#ifndef KLSCRIPTBINDING_HPP
#define KLSCRIPTBINDING_HPP

#include "../libbuild.hpp"

#include "../script/klscript.hpp"

#include <QObject>

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
class KLScriptbinding : public QObject, KLScript
{

		Q_OBJECT

	protected:

		QString LastCode;	//!< Ostatni przetwarzany skrypt.

		int LastLine;		//!< Ostatnia przetwarzana linia.

	public:

		using KLScript::Variables;
		using KLScript::Bindings;
		using KLScript::Parser;

		/*! \brief		Konstruktor.
		 *  \param [in]	Parent Rodzic obiektu.
		 *
		 * Buduje obiekt zgodnie z mechanizmem `QObject`.
		 *
		 */
		explicit KLScriptbinding(QObject* Parent = nullptr);

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
		double GetValue(void) const;

	signals:

		/*! \brief		Zdarzenie obliczenia wartości.
		 *  \param [in]	Value Wartość wyrażenia.
		 *  \see			Evaluate().
		 *
		 * Zdarzenie wywoływane przy poprawnym obliczeniu wartości wyrażenia.
		 *
		 */
		void onEvaluate(double);

};

#endif // KLSCRIPTBINDING_HPP
