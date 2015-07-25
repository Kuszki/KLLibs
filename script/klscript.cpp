/***********************************************************************
 *
 *  Lightweight Script Engine interpretation for KLLibs
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

#include "klscript.hpp"

KLScript::KLScript(void)
: LastReturn(0), LastError(NO_ERROR)
{
	Variables.Add("return", LastReturn);
}

KLScript::OPERATION KLScript::GetToken(const KLString& Script)
{
	KLString Token = GetName(Script);

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
	else if (Token == "return")	return T_RETURN;

	else if (Token == "exit")	return EXIT;

	else						return UNKNOWN;
}

KLString KLScript::GetParam(const KLString& Script)
{
	int Start = SkipComment(Script);

	while (Script[LastProcess] != ';' &&
		  Script[LastProcess] != '#' &&
		  Script[LastProcess] != ',' &&
		  Script[LastProcess] != '\0') ++LastProcess;

	KLString Param = Script.Part(Start, LastProcess);

	if (Script[LastProcess] == '#') return Param + GetParam(Script);

	while (isspace(Script[LastProcess])) ++LastProcess;

	return Param;
}

KLString KLScript::GetName(const KLString& Script)
{
	int Start = SkipComment(Script);

	while (isalpha(Script[LastProcess])) ++LastProcess;

	int Stop = LastProcess;

	if (Script[LastProcess] != ';' &&
	    Script[LastProcess] != ',')
	{
		while (isspace(Script[LastProcess])) ++LastProcess;
	}

	return Script.Part(Start, Stop);
}


bool KLScript::GetValue(const KLString& Script, KLVariables& Scoope)
{
	KLString Equation = GetParam(Script);

	for (KLVariables* Vars = &Scoope; Vars; Vars = Vars->Parent)
		for (auto& Var: *Vars) Equation.Replace(Var.ID, Var.Value.ToString(), true);

	return Parser.Evaluate(Equation);
}

int KLScript::SkipComment(const KLString& Script)
{
	while (isspace(Script[LastProcess])) ++LastProcess;

	if (Script[LastProcess] == '#')
	{
		while (Script[LastProcess] && Script[LastProcess] != '\n') ++LastProcess;

		SkipComment(Script);
	}

	return LastProcess;
}

bool KLScript::Evaluate(const KLString& Script)
{
	struct JUMP { int When; int Where; };

	KLVariables LocalVars(&Variables);
	KLList<JUMP> Jumps;

	LastError		= NO_ERROR;
	LastProcess	= 0;

	if (SkipComment(Script) == Script.Size()) ReturnError(WRONG_SCRIPTCODE);

	while (true)
	{
		if (Jumps.Size() && Jumps.Last().When == LastProcess) LastProcess = Jumps.Pop().Where;

		int Start = SkipComment(Script);

		switch (OPERATION ID = GetToken(Script))
		{
			case SET:
			{
				IF_Terminated ReturnError(WRONG_PARAMETERS);

				const KLString Var = GetName(Script);

				if (!LocalVars.Exists(Var)) ReturnError(UNDEFINED_VARIABLE);

				if (!GetValue(Script, LocalVars)) ReturnError(WRONG_EVALUATION);

				LocalVars[Var] = Parser.GetValue();
			}
			break;
			case CALL:
			{
				IF_Terminated ReturnError(WRONG_PARAMETERS);

				const KLString Proc = GetName(Script);

				if (!Bindings.Exists(Proc)) ReturnError(UNDEFINED_FUNCTION);

				KLVariables Params(&LocalVars);
				int ParamID = 0;

				if (!Terminated) do
				{
					if (!GetValue(Script, LocalVars)) ReturnError(WRONG_EVALUATION);

					KLString ID(ParamID++);

					Params.Add(ID);

					Params[ID] = Parser.GetValue();
				}
				while (IS_NextParam);

				if (IS_NoError) LastReturn = Bindings[Proc](Params);
			}
			break;
			case VAR:
			case EXP:
			{
				IF_Terminated ReturnError(WRONG_PARAMETERS);

				do
				{
					if (KLString Name = GetName(Script))
					{
						if (ID == VAR) LocalVars.Add(Name);
						else
						{
							if (LocalVars.Exists(Name))
							{
								Variables.Add(Name, LocalVars[Name]);
								LocalVars.Delete(Name);
							}
							else Variables.Add(Name);
						}
					}
					else ReturnError(EMPTY_EXPRESSION);
				}
				while (IS_NextParam);
			}
			break;
			case T_IF:
			{
				IF_Terminated ReturnError(WRONG_PARAMETERS);

				if (!GetValue(Script, LocalVars)) ReturnError(WRONG_EVALUATION);

				int Counter = 1;
				int Then = LastProcess;
				int Else = 0;

				while (Counter && LastProcess)
				{
					LastProcess = Script.Find(';', LastProcess) + 1;

					if (LastProcess) switch (GetToken(Script))
					{
						case T_IF:
							++Counter;
						break;
						case T_ELSE:
							if (Counter == 1) Else = LastProcess;
						break;
						case T_ENDIF:
							--Counter;
						break;
						default: break;
					}
				}

				if (Counter) ReturnError(EXPECTED_ENDIF_TOK);

				if (Parser.GetValue())
				{
					if (Else) Jumps.Insert({Else, LastProcess});

					LastProcess = Then;
				}
				else if (Else) LastProcess = Else;
			}
			break;
			case T_WHILE:
			{
				IF_Terminated ReturnError(WRONG_PARAMETERS);

				if (!GetValue(Script, LocalVars)) ReturnError(WRONG_EVALUATION);

				int Counter = 1;
				int Then = LastProcess;

				while (Counter && LastProcess)
				{
					LastProcess = Script.Find(';', LastProcess) + 1;

					if (LastProcess) switch (GetToken(Script))
					{
						case T_WHILE:
							++Counter;
						break;
						case T_DONE:
							--Counter;
						break;
						default: break;
					}
				}

				if (Counter) ReturnError(EXPECTED_DONE_TOK);

				if (Parser.GetValue())
				{
					Jumps.Insert({LastProcess + 1, Start});

					LastProcess = Then;
				}
			}
			break;

			case T_RETURN:
			{
				IF_Terminated ReturnError(WRONG_PARAMETERS);

				if (!GetValue(Script, LocalVars)) ReturnError(WRONG_EVALUATION);

				LastReturn = Parser.GetValue();

				return true;
			}
			break;

			case UNKNOWN:
				ReturnError(UNKNOWN_EXPRESSION);
			break;

			case END:
				if (LastProcess == Script.Size()) return IS_NoError;
				else ReturnError(EMPTY_EXPRESSION);
			break;

			case EXIT: return IS_NoError;

			default: break;
		}

		if (Terminated) ++LastProcess;
		else ReturnError(EXPECTED_TERMINATOR);

	}

	return true;
}

bool KLScript::Validate(const KLString& Script)
{
	KLVariables LocalVars(&Variables);

	LastError = NO_ERROR;
	LastProcess = 0;

	SkipComment(Script);

	if (SkipComment(Script) == Script.Size()) ReturnError(WRONG_SCRIPTCODE);

	while (true)
	{
		SkipComment(Script);

		switch (GetToken(Script))
		{
			case SET:
			{
				IF_Terminated ReturnError(WRONG_PARAMETERS);

				if (!LocalVars.Exists(GetName(Script))) ReturnError(UNDEFINED_VARIABLE);

				if (!GetValue(Script, LocalVars)) ReturnError(WRONG_EVALUATION);
			}
			break;
			case CALL:
			{
				IF_Terminated ReturnError(WRONG_PARAMETERS);

				if (!Bindings.Exists(GetName(Script))) ReturnError(UNDEFINED_FUNCTION);

				if (!Terminated) do
				{
					if (!GetValue(Script, LocalVars)) ReturnError(WRONG_EVALUATION);
				}
				while (IS_NextParam);
			}
			break;
			case VAR:
			case EXP:
			{
				IF_Terminated ReturnError(WRONG_PARAMETERS);

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
				IF_Terminated ReturnError(WRONG_PARAMETERS);

				if (!GetValue(Script, LocalVars)) ReturnError(WRONG_EVALUATION);

				int Counter = 1;
				int Then = LastProcess;

				while (Counter && LastProcess)
				{
					LastProcess = Script.Find(';', LastProcess) + 1;

					if (LastProcess) switch (GetToken(Script))
					{
						case T_IF:
							++Counter;
						break;
						case T_ENDIF:
							--Counter;
						break;
						default: break;
					}
				}

				if (Counter) ReturnError(EXPECTED_ENDIF_TOK);

				LastProcess = Then;
			}
			break;
			case T_WHILE:
			{
				IF_Terminated ReturnError(WRONG_PARAMETERS);

				if (!GetValue(Script, LocalVars)) ReturnError(WRONG_EVALUATION);

				int Counter = 1;
				int Then = LastProcess;

				while (Counter && LastProcess)
				{
					LastProcess = Script.Find(';', LastProcess) + 1;

					if (LastProcess) switch (GetToken(Script))
					{
						case T_WHILE:
							++Counter;
						break;
						case T_DONE:
							--Counter;
						break;
						default: break;
					}
				}

				if (Counter) ReturnError(EXPECTED_DONE_TOK);

				LastProcess = Then;
			}
			break;

			case T_RETURN:
			{
				IF_Terminated ReturnError(WRONG_PARAMETERS);

				if (!GetValue(Script, LocalVars)) ReturnError(WRONG_EVALUATION);
			}
			break;

			case UNKNOWN:
				ReturnError(UNKNOWN_EXPRESSION);
			break;

			case END:
				if (LastProcess == Script.Size()) return IS_NoError;
				else ReturnError(EMPTY_EXPRESSION);
			break;

			default: break;
		}

		if (Terminated) ++LastProcess;
		else ReturnError(EXPECTED_TERMINATOR);

	}

	return true;
}

KLScript::ERROR KLScript::GetError(void) const
{
	return LastError;
}

double KLScript::GetReturn(void) const
{
	return LastReturn;
}

const char* KLScript::GetMessage(void) const
{
	switch (LastError)
	{
		case UNDEFINED_VARIABLE:		return "Niezdefiniowana zmienna";
		case UNDEFINED_FUNCTION:		return "Niezdefiniowana funkcja";
		case EXPECTED_ENDIF_TOK:		return "Oczekiwano znacznika `fi`";
		case EXPECTED_DONE_TOK:		return "Oczekiwano znacznika `done`";
		case EXPECTED_TERMINATOR:	return "Oczekiwano terminatora `;`";
		case EMPTY_EXPRESSION:		return "Napotkano puste polecenie";
		case UNKNOWN_EXPRESSION:		return "Napotkano nieznane polecenie";
		case WRONG_SCRIPTCODE:		return "Niepoprawny lub pusty skrypt";
		case WRONG_PARAMETERS:		return "Niepoprawne parametry polecenia";
		case WRONG_EVALUATION:		return "Niepoprawny skrypt matematyczny";

		default: return "Skrypt jest poprawny";
	}
}

int KLScript::GetLine(const KLString& Script) const
{
	int Line = 1;

	for (int i = 0; i < LastProcess && i < Script.Size(); i++) if (Script[i] == '\n') ++Line;

	return Line;
}
