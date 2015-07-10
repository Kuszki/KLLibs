/***********************************************************************
 *
 *  KLScripteditor script code editor for KLLibs
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

#ifndef KLSCRIPTEDITOR_HPP
#define KLSCRIPTEDITOR_HPP

#include "../libbuild.hpp"

#include "klhighlighter.hpp"

#include <QPlainTextEdit>
#include <QTextBlock>
#include <QPainter>
#include <QObject>

/*! \file		klscripteditor.hpp
 *  \brief	Deklaracje dla klasy KLScripteditor i jej składników.
 *
 */

/*! \file		klscripteditor.cpp
 *  \brief	Implementacja klasy KLScripteditor i jej składników.
 *
 */

/*! \brief	Pole tekstowe do edycji skryptu `KLScript`.
 *
 * Klasa stanowi pole tekstowe rozszerzające `QPlainTextEdit` z automatycznym podświetlaniem składni, aktywnej linii i z wbudowanym numerowaniem linii.
 *
 */
class EXPORT KLScripteditor : public QPlainTextEdit
{

		Q_OBJECT

	protected: class KLScripteditorNumberarea final : public QWidget
	{

		protected:

			virtual void paintEvent(QPaintEvent* Event) override;

		public:

			explicit KLScripteditorNumberarea(KLScripteditor* Editor);

			QSize sizeHint(void) const override;

	};

	protected:

		KLScripteditorNumberarea* NumberArea;	//!< Widget numeracji wierszy.

		KLHighlighter* SyntaxHighlighter;		//!< Mechanizm podświetlania składni.

		virtual void resizeEvent(QResizeEvent *Event) override;

	public:

		/*! \brief		Konstruktor domyślny.
		 *  \param [in]	Parent Rodzic widgetu.
		 *
		 * Inicjuje pole rodzica i łączy potrzebne sygnały ze slotami.
		 *
		 */
		explicit KLScripteditor(QWidget* Parent = nullptr);

		/*! \brief		Destruktor.
		 *
		 * Zwalnia wszystkie użyte zasoby.
		 *
		 */
		virtual ~KLScripteditor(void) override;

		/*! \brief		Szerokość pola numeracji.
		 *  \return		Szerokość kolumny z numerami wierszy w pikselach.
		 *
		 * Oblicza i zwraca aktualną potrzebną szerokość dla kolumny z numerami wierszy.
		 *
		 */
		int numberareaWidth(void) const;

		/*! \brief		Mechanizm podświetlania tekstu.
		 *  \return		Instancja używanego mechanizmu podświetlania.
		 *
		 * Zwraca referencje mechanizmu używanego do podświetlania składni.
		 *
		 */
		KLHighlighter& Highlighter(void);

	private slots:

		void numberareaPaintEvent(QPaintEvent *event);

		void updateNumberareaWidth(int newBlockCount);
		void updateNumberareaView(const QRect& Rect, int dY);

		void highlightCurrentLine(void);

};

#endif // KLSCRIPTEDITOR_HPP
