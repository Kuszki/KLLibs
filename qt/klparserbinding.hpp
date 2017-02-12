/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  QT bindings for KLLib KLParser                                         *
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

#ifndef KLPARSERBINDING_HPP
#define KLPARSERBINDING_HPP

#include "../libbuild.hpp"

#include "../script/klparser.hpp"

#include <QObject>

/*! \file		klparserbinding.hpp
 *  \brief	Deklaracje dla klasy KLParserbinding i jej składników.
 *
 */

/*! \file		klparserbinding.cpp
 *  \brief	Implementacja klasy KLParserbinding i jej składników.
 *
 */

/*! \brief	Bindy QT do `KLParser`.
 *
 * Bazujący na `QObject` i `KLParser` bind ułatwiający współpracę z `KLParser` w środowisku QT.
 *
 */
class KLLIBS_EXPORT KLParserbinding : public QObject, KLParser
{

		Q_OBJECT

	public:

		/*! \brief		Konstruktor.
		 *  \param [in]	Parent Rodzic obiektu.
		 *
		 * Buduje obiekt zgodnie z mechanizmem `QObject`.
		 *
		 */
		explicit KLParserbinding(QObject* Parent = nullptr);

		/*! \brief		Destruktor.
		 *
		 * Niszczy obiekt zgodnie z mechanizmem `QObject`.
		 *
		 */
		virtual ~KLParserbinding(void) override;

		/*! \brief		Wywołanie skryptu.
		 *  \param [in]	Code		Wyrażenie do obliczenia.
		 *  \param [in]	Scoope	Zasięg zmiennych.
		 *  \return 		Powodzenie operacji.
		 *  \see			GetError(), GetValue().
		 *
		 * Przetwarza podane wyrażenie i zwraca powodzenie jego wykonania.
		 *
		 */
		bool Evaluate(const QString& Code, const KLVariables* Scoope = nullptr);

		/*! \brief		Pobranie wartości.
		 *  \return		Ostatnia poprawnie obliczona wartość.
		 *  \see			Evaluate(const QString&).
		 *
		 * Pobiera ostatnią obliczoną wartość.
		 *
		 */
		double GetValue(void) const;

		/*! \brief		Pobranie błędu.
		 *  \return		Ostatni napotkany błąd.
		 *  \see			Evaluate(const QString&).
		 *
		 * Pobiera ostatni błąd znaleziony w wyrażeniu.
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

		/*! \brief		Tłumaczenie kodu błędu.
		 *  \param [in]	Code Kod błędu.
		 *  \return		Czytelny opis błędu.
		 *
		 * Tłumaczy podany numer błędu na czytelny opis.
		 *
		 */
		static QString Errorcode(ERROR Code);

	signals:

		/*! \brief		Zdarzenie obliczenia wartości.
		 *  \param [in]	Value Wartość wyrażenia.
		 *  \see			Evaluate(const QString&).
		 *
		 * Zdarzenie wywoływane przy poprawnym obliczeniu wartości wyrażenia.
		 *
		 */
		void onEvaluate(double Value);

};

#endif // KLPARSERBINDING_HPP
