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

#include "klhighlighter.hpp"

QVector<KLHighlighter::KLHighlighterRule> KLHighlighter::Rules = QVector<KLHighlighter::KLHighlighterRule>();

KLHighlighter::KLHighlighter(QTextDocument* Parent)
: QSyntaxHighlighter(Parent)
{
	if (Rules.isEmpty())
	{
		KLHighlighterRule Rule;

		Rule.Format.setForeground(Qt::darkMagenta);
		Rule.Format.setFontWeight(QFont::Normal);

		Rule.Expresion = QRegExp("\\b[0-9]+\\b");

		Rules.append(Rule);

		Rule.Format.setForeground(Qt::darkBlue);
		Rule.Format.setFontWeight(QFont::Bold);

		Rule.Expresion = QRegExp("\\b(set|call|var|export|if|else|fi|while|done)\\b");

		Rules.append(Rule);

		Rule.Format.setForeground(Qt::darkBlue);
		Rule.Format.setFontWeight(QFont::Bold);

		Rule.Expresion = QRegExp("(\\+|\\-|\\*|\\/|\\%|\\^|\\=\\=|\\!\\=|\\>|\\<|\\>\\=|\\<\\=|\\&\\&|\\|\\||\\(|\\)|\\;|\\,)");

		Rules.append(Rule);

		Rule.Format.setForeground(Qt::darkRed);
		Rule.Format.setFontWeight(QFont::Bold);

		Rule.Expresion = QRegExp("\\b(return|exit)\\b");

		Rules.append(Rule);

		Rule.Format.setForeground(Qt::darkCyan);
		Rule.Format.setFontWeight(QFont::Bold);

		Rule.Expresion = QRegExp("\\b(sin|cos|tan|abs|exp|sqrt|log|ln|not|minus)\\b");

		Rules.append(Rule);

		Rule.Format.setForeground(Qt::darkGreen);
		Rule.Format.setFontWeight(QFont::Normal);
		Rule.Format.setFontItalic(true);

		Rule.Expresion = QRegExp("#[^\n]*");

		Rules.append(Rule);
	}
}

KLHighlighter::~KLHighlighter(void) {}

void KLHighlighter::highlightBlock(const QString& Text)
{
	foreach (const auto& Rule, Rules)
	{
		int Index = Rule.Expresion.indexIn(Text);

		while (Index >= 0)
		{
			int Length = Rule.Expresion.matchedLength();
			setFormat(Index, Length, Rule.Format);
			Index = Rule.Expresion.indexIn(Text, Index + Length);
		}
	}
}
