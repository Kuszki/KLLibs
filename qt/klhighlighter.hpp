/***********************************************************************
 *
 * KLScript code highlighter for KLLibs
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

#ifndef KLHIGHLIGHTER_HPP
#define KLHIGHLIGHTER_HPP

#include "../libbuild.hpp"

#include <QSyntaxHighlighter>
#include <QTextCharFormat>

/*! \file		klhighlighter.hpp
 *  \brief	Deklaracje dla klasy KLHighlighter i jej składników.
 *
 */

/*! \file		klhighlighter.cpp
 *  \brief	Implementacja klasy KLHighlighter i jej składników.
 *
 */

/*! \brief	Mechanizm wyróżniający elementy składni.
 *
 * Implementacja subklasy `QSyntaxHighlighter` umożliwiająca wyróżnianie składni `KLScript` w dokumentach Qt.
 *
 */
class EXPORT KLHighlighter : public QSyntaxHighlighter
{

		Q_OBJECT

	protected:

		/*! \brief		Reprezentacja wyróżnienia.
		 *
		 * Definiuje zasady wyróżnienia i jego styl.
		 *
		 */
		struct KLHighlighterRule
		{
			QTextCharFormat Format;	//!< Styl tekstu.
			QRegExp Expresion;		//!< Wyrażenie reguralne.
		};

		static QVector<KLHighlighterRule> Rules;	//!< Kontener na wyróżnienia.

		virtual void highlightBlock(const QString& Text) override;

	public:

		/*! \brief		Konstruktor domyślny.
		 *  \param [in]	Parent Dokument roboczy.
		 *
		 * Inicjuje pole rodzica i tworzy wrzystkie reguły wyróżnień o ile nie zostały już utworzone.
		 *
		 */
		KLHighlighter(QTextDocument* Parent);

		/*! \brief		Destruktor.
		 *
		 * Zwalnia wszystkie użyte zasoby.
		 *
		 */
		virtual ~KLHighlighter(void) override;

};

#endif // KLHIGHLIGHTER_HPP
