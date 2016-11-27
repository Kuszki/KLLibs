/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Lightweight Script Engine interpretation for KLLibs                    *
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

#include "klscript.hpp"

#define Terminated			(Script[LastProcess] == ';')
#define Separated			(Script[LastProcess] == ',')

#define IF_Terminated		if(Terminated)
#define IF_Separated		if(Separated)

#define IS_NoError			(LastError == NO_ERROR)
#define IS_NextParam		((Separated && IS_NoError) ? LastProcess++ : false)

#define ReturnError(error) 	{ LastError = error; return false; }

KLScript::KLScript(KLVariables* Scoope)
: Sigterm(false), LastReturn(0), LastError(NO_ERROR), Variables(Scoope) {}

KLScript::OPERATION KLScript::GetToken(const KLString& Script)
{
	KLString Token = GetName(Script);

	if (!Token)				return END;

	else if (Token == "set")		return SET;
	else if (Token == "call")	return CALL;
	else if (Token == "goto")	return GOTO;

	else if (Token == "var")		return VAR;
	else if (Token == "export")	return EXP;
	else if (Token == "pop")		return POP;

	else if (Token == "if")		return T_IF;
	else if (Token == "else")	return T_ELSE;
	else if (Token == "fi")		return T_ENDIF;

	else if (Token == "while")	return T_WHILE;
	else if (Token == "done")	return T_DONE;

	else if (Token == "define")	return T_DEF;
	else if (Token == "end")		return T_END;

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

	while (isalnum(Script[LastProcess])) ++LastProcess;

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

	Equation.Replace("return", LastReturn, true, true);

	return Parser.Evaluate(Equation, &Scoope);
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

bool KLScript::Evaluate(const KLString& Script, KLList<double>* Params)
{
	struct JUMP { int When; int Where; };

	KLVariables LocalVars(&Variables);
	KLList<JUMP> Jumps;

	LastError		= NO_ERROR;
	LastProcess	= 0;
	LastReturn	= NAN;
	Sigterm		= false;

	if (SkipComment(Script) == Script.Size()) ReturnError(WRONG_SCRIPTCODE);

	while (true)
	{
		if (Sigterm) ReturnError(SCRIPT_TERMINATED);

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

				KLVariables::KLVariable& Variable = LocalVars[Var];

				if (Variable.IsReadonly()) ReturnError(VARIABLE_READONLY);

				Variable = Parser.GetValue();
			}
			break;

			case CALL:
			{
				IF_Terminated ReturnError(WRONG_PARAMETERS);

				const KLString Proc = GetName(Script); KLList<double> Params;

				if (!Bindings.Exists(Proc)) ReturnError(UNDEFINED_FUNCTION);

				if (!Terminated) do
				{
					if (!GetValue(Script, LocalVars)) ReturnError(WRONG_EVALUATION);

					Params.Insert(Parser.GetValue());
				}
				while (IS_NextParam);

				if (IS_NoError) LastReturn = Bindings[Proc](Params);
			}
			break;

			case GOTO:
			{
				IF_Terminated ReturnError(WRONG_PARAMETERS);

				const KLString Proc = GetName(Script); KLList<double> Params;

				if (!Functions.Exists(Proc)) ReturnError(UNDEFINED_FUNCTION);

				if (!Terminated) do
				{
					if (!GetValue(Script, LocalVars)) ReturnError(WRONG_EVALUATION);

					Params.Insert(Parser.GetValue());
				}
				while (IS_NextParam);

				if (!IS_NoError) return false;
				else
				{
					int SavedLastProcess = LastProcess;

					Evaluate(Functions[Proc], &Params);

					LastProcess = SavedLastProcess;
				}
			}
			break;

			case VAR:
			case EXP:
			case POP:
			{
				IF_Terminated ReturnError(WRONG_PARAMETERS);

				do
				{
					if (KLString Name = GetName(Script))
					{
						if (ID == EXP)
						{
							if (LocalVars.Exists(Name, false))
							{
								Variables.Add(Name, LocalVars[Name]);
								LocalVars.Delete(Name);
							}
							else if (Variables.Parent && !Variables.Parent->Exists(Name))
							{
								Variables.Add(Name);
							}
						}
						else
						{
							LocalVars.Add(Name);
						}

						if (ID == POP && Params) LocalVars[Name] = Params->Dequeue();
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
					if (Else) Jumps.Insert({Else + 1, LastProcess + 1});

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

			case T_DEF:
			{
				IF_Terminated ReturnError(WRONG_PARAMETERS);

				const KLString Name = GetName(Script);

				if (Terminated) ++LastProcess;
				else ReturnError(WRONG_PARAMETERS);

				int Start = SkipComment(Script);
				int Stop = 0;
				int Counter = 1;

				while (Counter && LastProcess)
				{
					LastProcess = Stop = Script.Find(';', LastProcess) + 1;

					if (LastProcess) switch (GetToken(Script))
					{
						case T_DEF:
							++Counter;
						break;
						case T_END:
							--Counter;
						break;
						default: break;
					}
				}

				if (Counter) ReturnError(EXPECTED_DONE_TOK);

				const KLString Code = Script.Part(Start, Stop);
				int SavedLastProcess = LastProcess;

				if (!Code.Size()) ReturnError(EMPTY_FUNCTION);

				if (!Validate(Code, &LocalVars)) return false;
				else LastProcess = SavedLastProcess;

				if (Functions.Exists(Name)) Functions[Name] = Code;
				else Functions.Insert(Code, Name);
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

bool KLScript::Validate(const KLString& Script, KLVariables* Scoope)
{
	KLVariables LocalVars(Scoope ? Scoope : &Variables);

	LastError 	= NO_ERROR;
	LastProcess 	= 0;
	LastReturn	= NAN;

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

			case GOTO:
			{
				IF_Terminated ReturnError(WRONG_PARAMETERS);

				GetName(Script);

				if (!Terminated) do
				{
					if (!GetValue(Script, LocalVars)) ReturnError(WRONG_EVALUATION);
				}
				while (IS_NextParam);
			}
			break;

			case VAR:
			case EXP:
			case POP:
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

			case T_DEF:
			{
				IF_Terminated ReturnError(WRONG_PARAMETERS);

				GetName(Script);

				if (Terminated) ++LastProcess;
				else ReturnError(WRONG_PARAMETERS);

				int Start = SkipComment(Script);
				int Stop = 0;
				int Counter = 1;

				while (Counter && LastProcess)
				{
					LastProcess = Stop = Script.Find(';', LastProcess) + 1;

					if (LastProcess) switch (GetToken(Script))
					{
						case T_DEF:
							++Counter;
						break;
						case T_END:
							--Counter;
						break;
						default: break;
					}
				}

				if (Counter) ReturnError(EXPECTED_DONE_TOK);

				const KLString Code = Script.Part(Start, Stop);
				int SavedLastProcess = LastProcess;

				if (!Code.Size()) ReturnError(EMPTY_FUNCTION);

				if (!Validate(Code)) return false;
				else LastProcess = SavedLastProcess;
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

void KLScript::Terminate(void)
{
	Sigterm = true;
}

KLScript::ERROR KLScript::GetError(void) const
{
	return LastError;
}

double KLScript::GetReturn(void) const
{
	return LastReturn;
}

int KLScript::GetLine(const KLString& Script) const
{
	int Line = 1;

	for (int i = 0; i < LastProcess && i < Script.Size(); i++) if (Script[i] == '\n') ++Line;

	return Line;
}
