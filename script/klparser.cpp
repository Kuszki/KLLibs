/***********************************************************************
 *
 * Lightweight Math Parser interpretation for KLLibs
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

#include "klparser.hpp"

#include <QDebug>

thread_local KLParser::ERROR KLParser::KLParserToken::LastError = NO_ERROR;

const KLParser::KLParserOperator::KLParserOperatorData KLParser::KLParserOperator::Operators[] =
{
	{KLParser::KLParserOperator::UNKNOWN,	"",		00},

	{KLParser::KLParserOperator::ADD,		"+",		30},
	{KLParser::KLParserOperator::SUB,		"-",		30},
	{KLParser::KLParserOperator::MUL,		"*",		31},
	{KLParser::KLParserOperator::DIV,		"/",		31},
	{KLParser::KLParserOperator::MOD,		"%",		31},
	{KLParser::KLParserOperator::POW,		"^",		32},

	{KLParser::KLParserOperator::EQ,		"==",	20},
	{KLParser::KLParserOperator::NEQ,		"!=",	20},
	{KLParser::KLParserOperator::GT,		">",		21},
	{KLParser::KLParserOperator::LT,		"<",		21},
	{KLParser::KLParserOperator::GE,		">=",	21},
	{KLParser::KLParserOperator::LE,		"<=",	21},

	{KLParser::KLParserOperator::AND,		"&&",	10},
	{KLParser::KLParserOperator::OR,		"||",	11},

	{KLParser::KLParserOperator::L_BRACKET,	"(",		01},
	{KLParser::KLParserOperator::R_BRACKET,	")",		01}
};

const KLParser::KLParserFunction::KLParserFunctionData KLParser::KLParserFunction::Functions[] =
{
	{KLParser::KLParserFunction::UNKNOWN,	""},

	{KLParser::KLParserFunction::SIN,		"sin"},
	{KLParser::KLParserFunction::COS,		"cos"},
	{KLParser::KLParserFunction::TAN,		"tan"},

	{KLParser::KLParserFunction::ABS,		"abs"},

	{KLParser::KLParserFunction::EXP,		"exp"},
	{KLParser::KLParserFunction::SQRT,		"sqrt"},
	{KLParser::KLParserFunction::LOG,		"log"},
	{KLParser::KLParserFunction::LN,		"ln"},

	{KLParser::KLParserFunction::NOT,		"not"}
};

KLParser::KLParserToken::KLParserToken(void) {}

KLParser::KLParserToken::~KLParserToken(void) {}

KLParser::KLParserValue::KLParserValue(const char* Token)
{
	Value = atof(Token);
}

KLParser::KLParserValue::KLParserValue(double Token)
: Value(Token) {}

KLParser::KLParserValue::~KLParserValue(void) {}

unsigned KLParser::KLParserToken::GetPriority(void) const
{
	return 100;
}

KLParser::ERROR KLParser::KLParserToken::GetError(void) const
{
	return LastError;
}

double KLParser::KLParserValue::GetValue(void) const
{
	return Value;
}

KLParser::KLParserOperator::KLParserOperator(const char* Token)
: Operator(UNKNOWN)
{
	for (const auto& Symbol: Operators)
	{
		if (!strcmp(Token, Symbol.Token)) Operator = Symbol.Operator;
	}

	if (Operator == UNKNOWN) LastError = UNKNOWN_OPERATOR;
}


KLParser::KLParserOperator::KLParserOperator(OPERATOR Token)
: Operator(Token) {}

KLParser::KLParserOperator::~KLParserOperator(void) {}

double KLParser::KLParserOperator::GetValue(KLList<double>& Values) const
{
	LastError = NO_ERROR;

	if (Values.Size() < 1) LastError = NOT_ENOUGH_PARAMETERS;
	else
	{

		double ParamB = Values.Pop();
		double ParamA = Values.Pop();

		switch (Operator)
		{
			case ADD:
			return ParamA + ParamB;
			case SUB:
			return ParamA - ParamB;
			case MUL:
			return ParamA * ParamB;
			case DIV:
			return ParamA / ParamB;
			case MOD:
			return (int) ParamA % (int) ParamB;
			case POW:
			return pow(ParamA, ParamB);

			case EQ:
			return ParamA == ParamB;
			case NEQ:
			return ParamA != ParamB;
			case GT:
			return ParamA > ParamB;
			case LT:
			return ParamA < ParamB;
			case GE:
			return ParamA >= ParamB;
			case LE:
			return ParamA <= ParamB;

			case AND:
			return ParamA && ParamB;
			case OR:
			return ParamA || ParamB;

			default:
				LastError = UNKNOWN_OPERATOR;
		}

	}

	return 0;
}

KLParser::KLParserOperator::OPERATOR KLParser::KLParserOperator::GetOperator(void) const
{
	return Operator;
}

unsigned KLParser::KLParserOperator::GetPriority(void) const
{
	return Operators[Operator].Priority;
}

KLParser::KLParserFunction::KLParserFunction(const char* Token)
: Function(UNKNOWN)
{
	for (const auto& Symbol: Functions)
	{
		if (!strcmp(Token, Symbol.Token)) Function = Symbol.Function;
	}

	if (Function == UNKNOWN) LastError = UNKNOWN_FUNCTION;
}

KLParser::KLParserFunction::~KLParserFunction(void) {}

double KLParser::KLParserFunction::GetValue(KLList<double>& Values) const
{
	LastError = NO_ERROR;

	if (Values.Size() < 1) LastError = NOT_ENOUGH_PARAMETERS;
	else
	{

		double ParamA = Values.Pop();

		switch (Function)
		{
			case SIN:
			return sin(ParamA);
			case COS:
			return cos(ParamA);
			case TAN:
			return tan(ParamA);

			case ABS:
			return abs(ParamA);

			case EXP:
			return exp(ParamA);
			case SQRT:
			return sqrt(ParamA);
			case LOG:
			return log10(ParamA);
			case LN:
			return log(ParamA);

			default:
				LastError = UNKNOWN_FUNCTION;
		}

	}

	return 0;
}

KLParser::KLParserFunction::FUNCTION KLParser::KLParserFunction::GetFunction(void) const
{
	return Function;
}

bool KLParser::GetTokens(KLList<KLParserToken*>& Tokens, const KLString& Code)
{
	if (Code.Count('(') != Code.Count(')'))
	{
		LastError = BRACKETS_NOT_EQUAL;

		return false;
	}

	KLList<KLParserToken*> Operators;

	int Start = 0, Pos = 0;

	while (Pos < Code.Size() && LastError == NO_ERROR)
	{
		while (Pos < Code.Size() && isspace(Code[Pos])) Pos++;

		if (!Code[Pos]) break;
		else Start = Pos;

		if (isdigit(Code[Pos]))
		{
			while (isdigit(Code[Pos]) || Code[Pos] == '.') Pos++;

			Tokens << new KLParserValue(Code.Part(Start, Pos));
		}
		else if (isalpha(Code[Pos]))
		{
			while (isalpha(Code[Pos])) Pos++;

			Operators << new KLParserFunction(Code.Part(Start, Pos));
		}
		else
		{
			switch (Code[Pos])
			{
				case '(':
					Operators << new KLParserOperator(KLParserOperator::L_BRACKET);

					Pos++;
				break;
				case ')':
				{
					bool Continue = true;

					while (Continue)
					{
						KLParserToken* Token = Operators.Pop();

						if (KLParserOperator* Operator =
						    dynamic_cast<KLParserOperator*>(Token))
						{
							if (Operator->GetOperator() ==
							    KLParserOperator::L_BRACKET)
							{
								delete Operator;

								Continue = false;
								Token = nullptr;
							}
						}

						if (Token) Tokens << Token;
					}

					Pos++;
				}
				break;
				default:
				{
					while (Code[Pos] &&
						  !isspace(Code[Pos]) &&
						  !isdigit(Code[Pos]) &&
						  !isalpha(Code[Pos]) &&
						  Code[Pos] != '(' &&
						  Code[Pos] != ')') Pos++;

					KLParserToken* Operator =
							new KLParserOperator(Code.Part(Start, Pos));

					while (Operators.Size() &&
						  (Operator->GetPriority() <=
						   Operators[-1]->GetPriority()))
					{
						Tokens << Operators.Pop();
					}

					Operators << Operator;
				}
			}
		}
	}

	while (Operators.Size()) Tokens << Operators.Pop();

	return LastError == NO_ERROR;
}

bool KLParser::Evaluate(const KLString& Code)
{
	KLList<KLParserToken*> Tokens;
	KLList<double> Values;

	LastError = NO_ERROR;
	LastValue = 0;

	if (!GetTokens(Tokens, Code)) return false;

	for (auto& Token: Tokens)
	{
		if (KLParserValue* Operation = dynamic_cast<KLParserValue*>(Token))
		{
			Values << Operation->GetValue();
		}
		else if (KLParserOperator* Operation = dynamic_cast<KLParserOperator*>(Token))
		{
			Values << Operation->GetValue(Values);
		}
		else if (KLParserFunction* Operation = dynamic_cast<KLParserFunction*>(Token))
		{
			Values << Operation->GetValue(Values);
		}

		if ((LastError = Token->GetError()) != NO_ERROR) break;
	}

	if (LastError == NO_ERROR)
	{
		if (Values.Size() == 1) LastValue = Values.Pop();
		else LastError = TOO_MANY_PARAMETERS;
	}

	for (auto& Token: Tokens) delete Token;

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
