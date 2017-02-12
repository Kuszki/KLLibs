/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Lightweight Math Parser interpretation for KLLibs                      *
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

#include "klparser.hpp"

#define ReturnError(error) { LastError = error; return false; }

thread_local KLParser::ERROR KLParser::KLParserToken::LastError = KLParser::ERROR::NO_ERROR;

const KLParser::KLParserToken::KLParserOperatorData KLParser::KLParserToken::Operators[] =
{
	{ KLParser::KLParserToken::OPERATOR::UNKNOWN,	"",		00 },

	{ KLParser::KLParserToken::OPERATOR::ROUND,		"~",		31 },
	{ KLParser::KLParserToken::OPERATOR::ADD,		"+",		32 },
	{ KLParser::KLParserToken::OPERATOR::SUB,		"-",		32 },
	{ KLParser::KLParserToken::OPERATOR::MUL,		"*",		33 },
	{ KLParser::KLParserToken::OPERATOR::DIV,		"/",		33 },
	{ KLParser::KLParserToken::OPERATOR::MOD,		"%",		33 },
	{ KLParser::KLParserToken::OPERATOR::POW,		"^",		34 },

	{ KLParser::KLParserToken::OPERATOR::EQ,		"=",		21 },
	{ KLParser::KLParserToken::OPERATOR::NEQ,		"!=",	21 },
	{ KLParser::KLParserToken::OPERATOR::GT,		">",		22 },
	{ KLParser::KLParserToken::OPERATOR::LT,		"<",		22 },
	{ KLParser::KLParserToken::OPERATOR::GE,		">=",	22 },
	{ KLParser::KLParserToken::OPERATOR::LE,		"<=",	22 },

	{ KLParser::KLParserToken::OPERATOR::AND,		"&",		11 },
	{ KLParser::KLParserToken::OPERATOR::FAND,		"@",		11 },
	{ KLParser::KLParserToken::OPERATOR::OR,		"|",		12 },
	{ KLParser::KLParserToken::OPERATOR::FOR,		"?",		12 },

	{ KLParser::KLParserToken::OPERATOR::L_BRACKET,	"(",		01 },
	{ KLParser::KLParserToken::OPERATOR::R_BRACKET,	")",		01 }
};

const KLParser::KLParserToken::KLParserFunctionData KLParser::KLParserToken::Functions[] =
{
	{ KLParser::KLParserToken::FUNCTION::UNKNOWN,	""		},

	{ KLParser::KLParserToken::FUNCTION::SIN,		"sin"	},
	{ KLParser::KLParserToken::FUNCTION::COS,		"cos"	},
	{ KLParser::KLParserToken::FUNCTION::TAN,		"tan"	},

	{ KLParser::KLParserToken::FUNCTION::ABS,		"abs"	},

	{ KLParser::KLParserToken::FUNCTION::EXP,		"exp"	},
	{ KLParser::KLParserToken::FUNCTION::SQRT,		"sqrt"	},
	{ KLParser::KLParserToken::FUNCTION::LOG,		"log"	},
	{ KLParser::KLParserToken::FUNCTION::LN,		"ln"		},

	{ KLParser::KLParserToken::FUNCTION::NOT,		"not"	},
	{ KLParser::KLParserToken::FUNCTION::MINUS,		"minus"	}
};

KLParser::KLParserToken::KLParserToken(const char* Token, CLASS TokenClass)
: Class(TokenClass)
{
	switch (Class)
	{
		case CLASS::VALUE:
			Data.Value = atof(Token);
		break;
		case CLASS::OPERATOR:
			Data.Operator = OPERATOR::UNKNOWN;

			for (const auto& Symbol: Operators)
			{
				if (!strcmp(Token, Symbol.Token))
				{
					Data.Operator = Symbol.Operator; break;
				}
			}

			if (Data.Operator == OPERATOR::UNKNOWN) LastError = UNKNOWN_OPERATOR;
		break;
		case CLASS::FUNCTION:
			Data.Function = FUNCTION::UNKNOWN;

			for (const auto& Symbol: Functions)
			{
				if (!strcmp(Token, Symbol.Token))
				{
					Data.Function = Symbol.Function; break;
				}
			}

			if (Data.Function == FUNCTION::UNKNOWN) LastError = UNKNOWN_EXPRESSION;
		break;
	}
}

KLParser::KLParserToken::KLParserToken(double Value)
: Class(CLASS::VALUE)
{
	Data.Value = Value;
}

KLParser::KLParserToken::KLParserToken(OPERATOR Operator)
: Class(CLASS::OPERATOR)
{
	Data.Operator = Operator;
}

KLParser::KLParserToken::KLParserToken(FUNCTION Function)
: Class(CLASS::FUNCTION)
{
	Data.Function = Function;
}

unsigned KLParser::KLParserToken::GetPriority(void) const
{
	switch (Class)
	{
		case CLASS::OPERATOR:
			return Operators[(unsigned) Data.Operator].Priority;

		default: return 100;
	}

	return 0;
}

double KLParser::KLParserToken::GetValue(KLList<double>* Values) const
{
	static const auto roundto = [] (double Number, int To) -> double
	{
		if (To)
		{
			double Pow = pow(10, To);
			return round(Number * Pow) / Pow;
		}
		else return round(Number);
	};

	LastError = NO_ERROR;

	switch (Class)
	{
		case CLASS::OPERATOR:
			if (Values->Size() < 2) LastError = NOT_ENOUGH_PARAMETERS;
			else
			{
				double ParamB = Values->Pop();
				double ParamA = Values->Pop();

				switch (Data.Operator)
				{
					case OPERATOR::ROUND:	return roundto(ParamA, ParamB);
					case OPERATOR::ADD:		return ParamA + ParamB;
					case OPERATOR::SUB:		return ParamA - ParamB;
					case OPERATOR::MUL:		return ParamA * ParamB;
					case OPERATOR::DIV:		return ParamA / ParamB;
					case OPERATOR::MOD:		return int(ParamA) % int(ParamB);
					case OPERATOR::POW:		return pow(ParamA, ParamB);

					case OPERATOR::EQ:		return ParamA == ParamB;
					case OPERATOR::NEQ:		return ParamA != ParamB;
					case OPERATOR::GT:		return ParamA > ParamB;
					case OPERATOR::LT:		return ParamA < ParamB;
					case OPERATOR::GE:		return ParamA >= ParamB;
					case OPERATOR::LE:		return ParamA <= ParamB;

					case OPERATOR::AND:		return ParamA && ParamB;
					case OPERATOR::OR:		return ParamA || ParamB;

					case OPERATOR::FAND:	return fmin(ParamA, ParamB);
					case OPERATOR::FOR:		return fmax(ParamA, ParamB);

					default: LastError = UNKNOWN_OPERATOR;
				}
			}
		break;
		case CLASS::FUNCTION:
			if (Values->Size() < 1) LastError = NOT_ENOUGH_PARAMETERS;
			else
			{
				double ParamA = Values->Pop();

				switch (Data.Function)
				{
					case FUNCTION::SIN:		return sin(ParamA);
					case FUNCTION::COS:		return cos(ParamA);
					case FUNCTION::TAN:		return tan(ParamA);

					case FUNCTION::ABS:		return fabs(ParamA);

					case FUNCTION::EXP:		return exp(ParamA);
					case FUNCTION::SQRT:	return sqrt(ParamA);
					case FUNCTION::LOG:		return log10(ParamA);
					case FUNCTION::LN:		return log(ParamA);

					case FUNCTION::NOT:		return !ParamA;

					case FUNCTION::MINUS:	return -ParamA;

					default: LastError = UNKNOWN_EXPRESSION;
				}
			}
		break;
		default: return Data.Value;
	}

	return 0;
}

KLParser::KLParserToken::OPERATOR KLParser::KLParserToken::GetOperator(void) const
{
	switch (Class)
	{
		case CLASS::OPERATOR: return Data.Operator;

		default: return OPERATOR::UNKNOWN;
	}
}

KLParser::KLParserToken::FUNCTION KLParser::KLParserToken::GetFunction(void) const
{
	switch (Class)
	{
		case CLASS::FUNCTION: return Data.Function;

		default: return FUNCTION::UNKNOWN;
	}
}

KLParser::ERROR KLParser::KLParserToken::GetError(void) const
{
	return LastError;
}

bool KLParser::GetTokens(KLList<KLParserToken*>& Tokens, const KLString& Code, const KLVariables* Scoope)
{
	if (Code.Count('(') != Code.Count(')')) ReturnError(BRACKETS_NOT_EQUAL);

	KLList<KLParserToken*> Operators;
	KLParserToken* Operator = nullptr;

	bool isLastTokenOperator = true;
	int Start = 0, Pos = 0;

	while (Pos < Code.Size() && LastError == NO_ERROR)
	{
		while (Pos < Code.Size() && isspace(Code[Pos])) ++Pos;

		if (!Code[Pos]) break;
		else Start = Pos;

		if (isdigit(Code[Pos]))
		{
			isLastTokenOperator = false; while (isdigit(Code[Pos]) || Code[Pos] == '.') ++Pos;

			Tokens << new KLParserToken(Code.Part(Start, Pos), KLParserToken::CLASS::VALUE);
		}
		else if (isalpha(Code[Pos]))
		{
			isLastTokenOperator = true; while (isalnum(Code[Pos])) ++Pos;

			const KLString Name = Code.Part(Start, Pos);
			KLParserToken* Token = new KLParserToken(Name, KLParserToken::CLASS::FUNCTION);

			if (Token->GetFunction() == KLParserToken::FUNCTION::UNKNOWN)
			{
				delete Token; isLastTokenOperator = false;

				if (Scoope && Scoope->Exists(Name)) Tokens << new KLParserToken((*Scoope)[Name].ToNumber());
				else ReturnError(UNKNOWN_EXPRESSION);
			}
			else Operators << Token;
		}
		else
		{
			switch (Code[Pos])
			{
				case ')':
					do
					{
						if (Operators.Size()) Operator = Operators.Pop();
						else ReturnError(EXPECTED_BRACKET);

						if (Operator->GetOperator() == KLParserToken::OPERATOR::L_BRACKET)
						{
							delete Operator; Operator = nullptr;
						}
						else
						{
							Tokens << Operator;
						}
					}
					while (Operator);
				break;
				case '(':
					Operators << new KLParserToken(KLParserToken::OPERATOR::L_BRACKET);
				break;
				case '+':
					Operator = new KLParserToken(KLParserToken::OPERATOR::ADD);
				break;
				case '-':
					if (isLastTokenOperator) Operator = new KLParserToken(KLParserToken::FUNCTION::MINUS);
					else Operator = new KLParserToken(KLParserToken::OPERATOR::SUB);
				break;
				case '*':
					Operator = new KLParserToken(KLParserToken::OPERATOR::MUL);
				break;
				case '/':
					Operator = new KLParserToken(KLParserToken::OPERATOR::DIV);
				break;
				case '%':
					Operator = new KLParserToken(KLParserToken::OPERATOR::MOD);
				break;
				case '^':
					Operator = new KLParserToken(KLParserToken::OPERATOR::POW);
				break;
				default:
				{
					while (Code[Pos] &&
						  !isspace(Code[Pos]) &&
						  !isdigit(Code[Pos]) &&
						  !isalpha(Code[Pos]) &&
						  Code[Pos] != '(' &&
						  Code[Pos] != ')') ++Pos;

					Operator = new KLParserToken(Code.Part(Start, Pos--), KLParserToken::CLASS::OPERATOR);
				}
			}

			if (Operator)
			{
				while (Operators.Size() &&
					  (Operator->GetPriority() <=
					   Operators.Last()->GetPriority()))
				{
					Tokens << Operators.Pop();
				}

				Operators << Operator;

				isLastTokenOperator = true;
				Operator = nullptr;
			}

			++Pos;
		}
	}

	while (Operators.Size()) Tokens << Operators.Pop();

	return LastError == NO_ERROR;
}

bool KLParser::Evaluate(const KLString& Code, const KLVariables* Scoope)
{
	KLList<KLParserToken*> Tokens;
	KLList<double> Values;

	LastError = NO_ERROR;
	LastValue = 0;

	if (!GetTokens(Tokens, Code, Scoope)) return false;

	for (auto& Token: Tokens)
	{
		Values << Token->GetValue(&Values);
		LastError = Token->GetError();

		if (LastError) break;
	}

	if (LastError == NO_ERROR)
	{
		if (Values.Size() == 1) LastValue = Values.Pop();
		else LastError = TOO_MANY_PARAMETERS;
	}

	for (auto Token: Tokens) delete Token;

	return LastError == NO_ERROR;
}

double KLParser::GetValue(void) const
{
	return LastValue;
}

KLParser::ERROR KLParser::GetError(void) const
{
	return LastError;
}
