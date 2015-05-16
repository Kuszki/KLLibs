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

#include "klscript.hpp"

#include <QDebug>

KLScript::KLScript(void)
{
	Variables.Add("return");
}

KLScript::OPERATION KLScript::GetToken(const KLString& Code)
{
	while (isspace(Code[Process])) Process++;

	if (!Code[Process]) return CODE_END;

	int Start = Process;

	while (isalpha(Code[Process]) &&
		  Code[Process] != '\0' &&
		  Code[Process] != ';') Process++;

	KLString Token = Code.Part(Start, Process++);

	if (Token == "#") return COMMENT;
	else if (Token == "var") return VAR;
	else if (Token == "set") return SET;
	else if (Token == "call") return CALL;
	else if (Token == "if") return T_IF;
	else if (Token == "else") return T_ELSE;
	else if (Token == "fi") return T_ENDIF;
	else if (Token == "while") return T_WHILE;
	else if (Token == "done") return T_DONE;
	else if (Token == "exit") return T_EXIT;

	LastError = UNKNOWN_EXPRESSION;

	return UNKNOWN;
}

KLString KLScript::GetParam(const KLString& Code)
{
	while (isspace(Code[Process])) Process++;

	int Start = Process;

	while (!isspace(Code[Process]) &&
		  Code[Process] != '\0' &&
		  Code[Process] != ';' &&
		  Code[Process] != ',') Process++;

	return Code.Part(Start, Process++);
}

bool KLScript::GetValue(const KLString& Code)
{
	while (isspace(Code[Process])) Process++;

	int Start = Process;

	while (Code[Process] != '\0' &&
		  Code[Process] != ';' &&
		  Code[Process] != ',') Process++;

	KLString Equation = Code.Part(Start, Process++);

	for (auto& Var: Variables) Equation.Replace(Var.ID, Var.Value.ToString(), true);

	if (!Parser.Evaluate(Equation)) LastError = WRONG_EVALUATION;

	return LastError == NO_ERROR;
}

bool KLScript::Evaluate(const KLString& Code)
{
	KLVariables LocalVars(&Variables);

	struct JUMP
	{
		int When;
		int Where;
	};

	KLList<JUMP> Jumps;

	LastError = NO_ERROR;
	Process = 0;

	while (Process <= Code.Size() && LastError == NO_ERROR)
	{
		if (Jumps.Size()) if (Jumps[-1].When == Process) Process = Jumps.Pop().Where;

		int Start = Process;

		switch (GetToken(Code))
		{
			case COMMENT:
				while (Code[Process] && Code[Process] != '\n') Process++;
			break;
			case VAR:
			{
				unsigned Count = 0;

				while (Code[Process - 1] != ';' && LastError == NO_ERROR)
				{
					if (KLString Name = GetParam(Code))
					{
						LocalVars.Add(Name);

						Count++;
					}
				}

				if (!Count) LastError = MISSING_PARAMETERS;
			}
			break;
			case SET:
			{
				KLString Var = GetParam(Code);

				if (!LocalVars.Exists(Var)) LastError = UNDEFINED_VARIABLE;
				else if (GetValue(Code)) LocalVars[Var] = Parser.GetValue();
			}
			break;
			case CALL:
			{
				KLString Proc = GetParam(Code);

				if (!Bindings.Exists(Proc)) LastError = UNDEFINED_FUNCTION;
				else
				{
					KLVariables Params(&LocalVars);

					int Number = 0;

					while (Code[Process - 1] != ';' && LastError == NO_ERROR)
					{
						if (GetValue(Code))
						{
							KLString ID(Number++);

							Params.Add(ID);

							Params[ID] = Parser.GetValue();
						}

						Process++;
					}

					if (LastError == NO_ERROR) Variables["return"] = Bindings[Proc](Params);
				}
			}
			break;
			case T_IF:
			{
				int LastProcess = Process;
				int Counter = 1;

				int Then = 0, Else = 0, Endif = 0;

				do
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
				while (Counter && Process != -1);

				if (Counter) LastError = EXPECTED_ENDIF_TOK;
				else
				{
					Endif = Process;
					Process = LastProcess;

					if (GetValue(Code))
					{
						bool Boolean = Parser.GetValue();

						Then = Process;

						if (Else)
						{
							if (Boolean)
							{
								Process = Then;

								Jumps.Insert({Else, Endif});
							}
							else
							{
								Process = Else;
							}
						}
						else
						{
							if (Boolean)
							{
								Process = Then;
							}
							else
							{
								Process = Endif;
							}
						}
					}
				}
			}
			break;
			case T_WHILE:
			{
				int LastProcess = Process;
				int Counter = 1;

				int Done = 0;

				do
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
				while (Counter && Process != -1);

				if (Counter) LastError = EXPECTED_DONE_TOK;
				else
				{
					Done = Process;
					Process = LastProcess;

					if (GetValue(Code))
					{
						bool Boolean = Parser.GetValue();

						if (Boolean)
						{
							Jumps.Insert({Done, Start});
						}
						else
						{
							Process = Done;
						}
					}
				}
			}
			break;
			case T_EXIT:
			case CODE_END:
				return true;
			case UNKNOWN:
				return false;

			default: break;
		}
	}

	return LastError == NO_ERROR;
}

KLScript::ERROR KLScript::GetError(void) const
{
	return LastError;
}
