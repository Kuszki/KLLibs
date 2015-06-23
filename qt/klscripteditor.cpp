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

#include "klscripteditor.hpp"

KLScripteditor::KLScripteditorNumberarea::KLScripteditorNumberarea(KLScripteditor* Editor)
: QWidget(Editor) {}

void KLScripteditor::KLScripteditorNumberarea::paintEvent(QPaintEvent* Event)
{
	auto Parent = qobject_cast<KLScripteditor*>(parent());

	Parent->numberareaPaintEvent(Event);
}

QSize KLScripteditor::KLScripteditorNumberarea::sizeHint(void) const
{
	auto Parent = qobject_cast<KLScripteditor*>(parent());

	return QSize(Parent->numberareaWidth(), 0);
}

KLScripteditor::KLScripteditor(QWidget* Parent)
: QPlainTextEdit(Parent)
{
	SyntaxHighlighter = new KLHighlighter(this->document());
	NumberArea = new KLScripteditorNumberarea(this);

	connect(this, SIGNAL(blockCountChanged(int)),
		   this, SLOT(updateNumberareaWidth(int)));

	connect(this, SIGNAL(updateRequest(QRect,int)),
		   this, SLOT(updateNumberareaView(QRect,int)));

	connect(this, SIGNAL(cursorPositionChanged()),
		   this, SLOT(highlightCurrentLine()));

	updateNumberareaWidth(0);
	highlightCurrentLine();
	setTabStopWidth(25);
}

KLScripteditor::~KLScripteditor(void) {}

int KLScripteditor::numberareaWidth(void) const
{
	int digits = 1;
	int max = qMax(1, blockCount());

	while (max >= 10)
	{
		max /= 10;
		++digits;
	}

	int space = 6 + fontMetrics().width(QLatin1Char('9')) * digits;

	return space;
}

KLHighlighter& KLScripteditor::Highlighter(void)
{
	return *SyntaxHighlighter;
}

void KLScripteditor::numberareaPaintEvent(QPaintEvent* Event)
{
	QTextBlock block = firstVisibleBlock();
	QPainter painter(NumberArea);

	painter.fillRect(Event->rect(), QColor(235, 235, 235));
	painter.setPen(Qt::black);

	int blockNumber = block.blockNumber();
	int top = blockBoundingGeometry(block).translated(contentOffset()).top();
	int bottom = top + blockBoundingRect(block).height();

	while (block.isValid() && top <= Event->rect().bottom())
	{
		if (block.isVisible() && bottom >= Event->rect().top())
		{
			painter.drawText(0,
						  top,
						  NumberArea->width() - 3,
						  fontMetrics().height(),
						  Qt::AlignRight,
						  QString::number(blockNumber + 1));
		}

		block = block.next();
		top = bottom;
		bottom = top + blockBoundingRect(block).height();

		++blockNumber;
	}
}

void KLScripteditor::updateNumberareaWidth(int)
{
	setViewportMargins(numberareaWidth(), 0, 0, 0);
}

void KLScripteditor::updateNumberareaView(const QRect& Rect, int dY)
{
	if (dY)
		NumberArea->scroll(0,
					    dY);
	else
		NumberArea->update(0,
					    Rect.y(),
					    NumberArea->width(),
					    Rect.height());

	if (Rect.contains(viewport()->rect())) updateNumberareaWidth(0);
}

void KLScripteditor::resizeEvent(QResizeEvent* Event)
{
	QPlainTextEdit::resizeEvent(Event);

	QRect cr = contentsRect();

	NumberArea->setGeometry(QRect(cr.left(),
							cr.top(),
							numberareaWidth(),
							cr.height()));
}

void KLScripteditor::highlightCurrentLine(void)
{
	QList<QTextEdit::ExtraSelection> extraSelections;

	if (!isReadOnly())
	{
		QTextEdit::ExtraSelection selection;

		selection.format.setBackground(QColor(255, 255, 205));
		selection.format.setProperty(QTextFormat::FullWidthSelection, true);
		selection.cursor = textCursor();
		selection.cursor.clearSelection();
		extraSelections.append(selection);
	}

	setExtraSelections(extraSelections);
}
