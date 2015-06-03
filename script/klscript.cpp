/***********************************************************************
 *
 * Lightweight Script Engine interpretation for KLLibs
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

#include "klscript.hpp"

#include <QDebug>

KLScript::KLScript(const KLString& Script)
: Code(Script)
{
	#ifdef QT_VERSION
		setlocale(LC_NUMERIC, "POSIX");
	#endif

	Variables.Add("return");

	//if (Code) if (!Validate(Code)) Code.Clean();
}

KLScript::OPERATION KLScript::GetToken(const KLString& Script)
{
	KLString Token = GetName(Script);

	qDebug() << "EXPR:" << Token;

	if (!Token)				return END;

	else if (Token == "set")		return SET;
	else if (Token == "call")	return CALL;
	else if (Token == "var")		return VAR;
	else if (Token == "export")	return EXP;
	else if (Token == "if")		return T_IF;
	else if (Token == "else")	return T_ELSE;
	else if (Token == "fi")		return T_ENDIF;
	else if (Token == "while")	return T_WHILE;
	else if (Token == "done")	return T_DONE;
	else if (Token == "exit")	return EXIT;

	else						return UNKNOWN;
}

KLString KLScript::GetParam(const KLString& Script)
{
	int Start = SkipComment(Script);

	while (Script[Process] != ';' &&
		  Script[Process] != '#' &&
		  Script[Process] != ',' &&
		  Script[Process] != '\0') Process++;

	KLString Param = Script.Part(Start, Process);

	if (Script[Process] == '#') return Param + GetParam(Script);

	while (isspace(Script[Process])) Process++;

	return Param;
}

KLString KLScript::GetName(const KLString& Script)
{
	int Start = SkipComment(Script);

	while (isalpha(Script[Process])) Process++;

	int Stop = Process;

	if (Script[Process] != ';' &&
	    Script[Process] != ',')
	{
		while (isspace(Script[Process])) Process++;
	}

	return Script.Part(Start, Stop);
}


bool KLScript::GetValue(const KLString& Script, KLVariables& Scoope)
{
	KLString Equation = GetParam(Script);

	for (KLVariables* Vars = &Scoope; Vars; Vars = Vars->Parent)
		for (auto& Var: *Vars) Equation.Replace(Var.ID, Var.Value.ToString(), true);

	qDebug() << Equation;

	return Parser.Evaluate(Equation);
}

int KLScript::SkipComment(const KLString& Script)
{
	while (isspace(Script[Process])) Process++;

	if (Script[Process] == '#')
	{
		while (Script[Process] && Script[Process] != '\n') Process++;

		SkipComment(Script);
	}

	return Process;
}

bool KLScript::Validate(const KLString& Script)
{
	KLVariables LocalVars;

	LastError = NO_ERROR;
	Process = 0;

	while (Process < Script.Size() && IS_NoError)
	{
		switch (GetToken(Script))
		{
			case SET:
			{
				IF_Terminator ReturnError(WRONG_PARAMETERS);

				KLString Var = GetName(Script);

				if (!LocalVars.Exists(Var)) ReturnError(UNDEFINED_VARIABLE);
				if (!GetValue(Script, LocalVars)) ReturnError(WRONG_EVALUATION);
			}
			break;
			case CALL:
			{
				IF_Terminator ReturnError(WRONG_PARAMETERS);

				GetName(Script);

				do (GetValue(Script, LocalVars));
				while (IS_NextParam);
			}
			break;
			case VAR:
			case EXP:
			{
				IF_Terminator ReturnError(WRONG_PARAMETERS);

				do
				{
					if (KLString Name = GetName(Script))
					{
						LocalVars.Add(Name);
					}
					else ReturnError(EMPTY_EXPRESSION);
				}
				while (IS_NextParam);
			}
			break;
			case T_IF:
			{
				IF_Terminator ReturnError(WRONG_PARAMETERS);

				if (!GetValue(Script, LocalVars)) ReturnError(WRONG_EVALUATION);

				int Counter = 1;

				while (Counter && Process != -1)
				{
					Process = Script.Find(';', Process) + 1;

					switch (GetToken(Script))
					{
						case T_IF:
							Counter++;
						break;
						case T_ENDIF:
							Counter--;
						break;
						default: break;
					}
				}

				if (Counter) ReturnError(EXPECTED_ENDIF_TOK);
			}
			break;
			case T_WHILE:
			{
				IF_Terminator ReturnError(WRONG_PARAMETERS);

				if (!GetValue(Script, LocalVars)) ReturnError(WRONG_EVALUATION);

				int Counter = 1;

				while (Counter && Process != -1)
				{
					Process = Script.Find(';', Process) + 1;

					switch (GetToken(Script))
					{
						case T_WHILE:
							Counter++;
						break;
						case T_DONE:
							Counter--;
						break;
						default: break;
					}
				}

				if (Counter) ReturnError(EXPECTED_DONE_TOK);
			}
			break;

			case UNKNOWN: ReturnError(UNKNOWN_EXPRESSION);

			case END: return IS_NoError;

			default: continue;
		}

		if (Script[Process] == ';') Process++;
		else ReturnError(EXPECTED_TERMINATOR);

	}

	return IS_NoError;
}

bool KLScript::Evaluate(void)
{
	if (!Code) ReturnError(WRONG_SCRIPTCODE);

	struct JUMP { int When; int Where; };

	KLVariables LocalVars(&Variables);
	KLList<JUMP> Jumps;

	LastError = NO_ERROR;
	Process = 0;

	while (Process < Code.Size() && IS_NoError)
	{
		if (Jumps.Size()) if (Jumps[-1].When == Process) Process = Jumps.Pop().Where;

		int Start = SkipComment(Code);

		switch (OPERATION ID = GetToken(Code))
		{
			case SET:
			{
				IF_Terminator ReturnError(WRONG_PARAMETERS);

				KLString Var = GetName(Code);

				if (!LocalVars.Exists(Var))
					ReturnError(UNDEFINED_VARIABLE)
				else if (!GetValue(Code, LocalVars))
					ReturnError(WRONG_EVALUATION)
				else LocalVars[Var] = Parser.GetValue();
			}
			break;
			case CALL:
			{
				IF_Terminator ReturnError(WRONG_PARAMETERS);

				const KLString Proc = GetName(Code);

				if (!Bindings.Exists(Proc)) ReturnError(UNDEFINED_FUNCTION);

				KLVariables Params(&LocalVars);
				int ParamID = 0;

				do if (GetValue(Code, LocalVars))
				{
					KLString ID(ParamID++);

					Params.Add(ID);

					Params[ID] = Parser.GetValue();
				}
				while (IS_NextParam);

				if (!IS_NoError) Variables["return"] = Bindings[Proc](Params);
			}
			break;
			case VAR:
			case EXP:
			{
				IF_Terminator ReturnError(WRONG_PARAMETERS);

				do
				{
					if (KLString Name = GetName(Code))
					{
						if (ID == VAR) LocalVars.Add(Name);
						else
						{
							if (LocalVars.Exists(Name))
								Variables.Add(Name, LocalVars[Name]);
							else
								Variables.Add(Name);
						}
					}
					else ReturnError(EMPTY_EXPRESSION);
				}
				while (IS_NextParam);
			}
			break;
			case T_IF:
			{
				IF_Terminator ReturnError(WRONG_PARAMETERS);

				if (!GetValue(Code, LocalVars)) ReturnError(WRONG_EVALUATION);

				int Counter = 1;
				int Then = Process;
				int Else = 0;

				while (Counter && Process != -1)
				{
					Process = Code.Find(';', Process) + 1;

					switch (GetToken(Code))
					{
						case T_IF:
							Counter++;
						break;
						case T_ELSE:
							if (Counter == 1) Else = Process;
						break;
						case T_ENDIF:
							Counter--;
						break;
						default: break;
					}
				}

				if (Counter) ReturnError(EXPECTED_ENDIF_TOK);

				if (Parser.GetValue())
				{
					if (Else) Jumps.Insert({Else, Process});

					Process = Then;
				}
				else if (Else) Process = Else;
			}
			break;
			case T_WHILE:
			{
				IF_Terminator ReturnError(WRONG_PARAMETERS);

				if (!GetValue(Code, LocalVars)) ReturnError(WRONG_EVALUATION);

				int Counter = 1;
				int Then = Process;

				while (Counter && Process != -1)
				{
					Process = Code.Find(';', Process) + 1;

					switch (GetToken(Code))
					{
						case T_WHILE:
							Counter++;
						break;
						case T_DONE:
							Counter--;
						break;
						default: break;
					}
				}

				if (Counter) ReturnError(EXPECTED_DONE_TOK);

				if (Parser.GetValue())
				{
					Jumps.Insert({Process, Start});

					Process = Then;
				}
			}
			break;

			case UNKNOWN: ReturnError(UNKNOWN_EXPRESSION);

			case EXIT:
			case END:
				return IS_NoError;

			default: continue;
		}

		qDebug() << Code[Process];

		if (Code[Process] == ';') Process++;
		else ReturnError(EXPECTED_TERMINATOR);

	}

	return IS_NoError;
}

KLScript::ERROR KLScript::GetError(void) const
{
	return LastError;
}

bool KLScript::SetCode(const KLString& Script)
{
	//if (Validate(Script)) Code = Script;
	//else return false;

	Code = Script;

	return true;
}
