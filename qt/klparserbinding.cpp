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

#include "klparserbinding.hpp"

KLParserbinding::KLParserbinding(QObject* Parent)
: QObject(Parent), KLParser() {}

KLParserbinding::~KLParserbinding(void) {}

bool KLParserbinding::Evaluate(const QString& Code, const KLVariables* Scoope)
{
	const bool OK = KLParser::Evaluate(Code.toStdString().c_str(), Scoope);

	if (OK) emit onEvaluate(LastValue);

	return OK;
}

double KLParserbinding::GetValue(void) const
{
	return LastValue;
}

KLParserbinding::ERROR KLParserbinding::GetError(void) const
{
	return LastError;
}

QString KLParserbinding::GetMessage(void) const
{
	return Errorcode(LastError);
}

QString KLParserbinding::Errorcode(KLParser::ERROR Code)
{
	switch (Code)
	{
		case UNEXPECTED_OPERATOR:	return tr("Encountered unexpected operator");
		case UNKNOWN_OPERATOR:		return tr("Encountered unknown operator");
		case UNKNOWN_EXPRESSION:		return tr("Encountered unknown symbol");
		case NOT_ENOUGH_PARAMETERS:	return tr("Expected paramters");
		case TOO_MANY_PARAMETERS:	return tr("Expected operator");
		case BRACKETS_NOT_EQUAL:		return tr("Encountered single bracket");

		default: return tr("Script is valid");
	}
}
