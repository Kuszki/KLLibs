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

	/*! \brief		Wyliczenie styli wyróżnienia.
	 *
	 * Pozwala ograniczyć pule styli do wybranych pozycji.
	 *
	 */
	public: enum STYLE
	{
		NUMBERS,		//!< Liczby.
		KEYWORDS,		//!< Słowa kluczowe.
		OPERATORS,	//!< Operatory matematyczne.
		BREAKS,		//!< Słowa kluczowe kończące program.
		MATHS,		//!< Funkcje matematyczne.
		COMMENTS		//!< Komentarze.
	};

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

		QMap<STYLE, KLHighlighterRule> Rules;	//!< Kontener na wyróżnienia.

		virtual void highlightBlock(const QString& Text) override;

	public:

		/*! \brief		Konstruktor domyślny.
		 *  \param [in]	Parent Dokument roboczy.
		 *
		 * Inicjuje pole rodzica i tworzy wrzystkie domyślne reguły wyróżnień.
		 *
		 */
		KLHighlighter(QTextDocument* Parent);

		/*! \brief		Destruktor.
		 *
		 * Zwalnia wszystkie użyte zasoby.
		 *
		 */
		virtual ~KLHighlighter(void) override;

		/*! \brief		Ustala styl.
		 *  \param [in]	Style	Indeks stylu.
		 *  \param [in]	Format	Format stylu.
		 *
		 * Ustala nowy styl dla wybranego fragmentu kodu.
		 *
		 */
		void SetFormat(STYLE Style,
					const QTextCharFormat& Format);

		/*! \brief		Pobiera styl.
		 *  \param [in]	Style Indeks stylu.
		 *  \return		Styl wybranego fragmentu.
		 *
		 * Pobiera styl wybranego fragmentu kodu.
		 *
		 */
		QTextCharFormat GetFormat(STYLE Style) const;

};

#endif // KLHIGHLIGHTER_HPP
