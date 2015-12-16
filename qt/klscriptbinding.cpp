/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  QT bindings for KLLib KLScript                                         *
 *  Copyright (C) 2015  Łukasz "Kuszki" Dróżdż            l.drozdz@o2.pl   *
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

#include "klscriptbinding.hpp"

KLScriptbinding::KLScriptbinding(KLVariables* Scoope, QObject* Parent)
: QObject(Parent), KLScript(Scoope)
{
	std::setlocale(LC_NUMERIC, "POSIX");
}

KLScriptbinding::~KLScriptbinding(void) {}

void KLScriptbinding::SetCode(const QString& Script)
{
	LastCode = Script;
}

QString KLScriptbinding::GetCode(void) const
{
	return LastCode;
}

bool KLScriptbinding::Evaluate(void)
{
	const bool OK = KLScript::Evaluate(LastCode.toStdString().c_str());

	LastLine = KLScript::GetLine(LastCode.toStdString().c_str());

	emit onEvaluate(LastReturn, LastError);

	return OK;
}

bool KLScriptbinding::Validate(const QString& Script)
{
	const bool OK = KLScript::Validate(Script.toStdString().c_str());

	LastLine = KLScript::GetLine(Script.toStdString().c_str());

	return OK;
}

void KLScriptbinding::Optimize(void)
{
	for (auto& Code: Functions) Code.Value = Optimize(QString((const char*) Code.Value)).toStdString().c_str();

	LastCode = Optimize(LastCode);
}

KLScriptbinding::ERROR KLScriptbinding::GetError(void) const
{
	return LastError;
}

QString KLScriptbinding::GetMessage(void) const
{
	switch (LastError)
	{
		case UNDEFINED_VARIABLE:		return tr("Encountered undefined variable");
		case UNDEFINED_FUNCTION:		return tr("Encountered undefined function bind");
		case EXPECTED_ENDIF_TOK:		return tr("Expected `fi` token");
		case EXPECTED_DONE_TOK:		return tr("Expected `done` token");
		case EXPECTED_TERMINATOR:	return tr("Expected `;` token");
		case EMPTY_FUNCTION:		return tr("Encountered empty function");
		case EMPTY_EXPRESSION:		return tr("Encountered empty expression");
		case UNKNOWN_EXPRESSION:		return tr("Encountered unknown expression");
		case WRONG_SCRIPTCODE:		return tr("Wrong or empty Scriptcode");
		case WRONG_PARAMETERS:		return tr("Encountered invalid expresion parameters");
		case WRONG_EVALUATION:		return tr("Encountered invalid math expresion");
		case VARIABLE_READONLY:		return tr("Selected variable is readonly");
		case SCRIPT_TERMINATED:		return tr("Script terminated before end");

		default: return tr("Script is valid");
	}
}

int KLScriptbinding::GetLine(void) const
{
	return LastLine;
}

double KLScriptbinding::GetReturn(void) const
{
	return LastReturn;
}

QString KLScriptbinding::Optimize(const QString& Script)
{
	return QString(Script)
			.remove(QRegExp("\\s*#[^\n]*\\s*"))
			.replace(QRegExp("\\s+(\\W)\\s+|(\\W)\\s+|\\s+(\\W)"), QString("\\1\\2\\3"))
			.replace(QRegExp("\\s+"), " ");
}
