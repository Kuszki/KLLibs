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

const KLParser::KLParserOperator::KLParserOperatorData KLParser::KLParserOperator::Operators[] =
{
	{KLParser::KLParserOperator::UNKNOWN,	"",		0},

	{KLParser::KLParserOperator::ADD,		"+",		5},
	{KLParser::KLParserOperator::SUB,		"-",		5},
	{KLParser::KLParserOperator::MUL,		"*",		6},
	{KLParser::KLParserOperator::DIV,		"/",		6},
	{KLParser::KLParserOperator::MOD,		"%",		7},
	{KLParser::KLParserOperator::POW,		"^",		7},

	{KLParser::KLParserOperator::EQ,		"==",	3},
	{KLParser::KLParserOperator::NEQ,		"!=",	3},
	{KLParser::KLParserOperator::GT,		">",		4},
	{KLParser::KLParserOperator::LT,		"<",		4},
	{KLParser::KLParserOperator::GE,		">=",	4},
	{KLParser::KLParserOperator::LE,		"<=",	4},

	{KLParser::KLParserOperator::AND,		"&&",	3},
	{KLParser::KLParserOperator::OR,		"||",	2},

	{KLParser::KLParserOperator::NOT,		"!",		10},

	{KLParser::KLParserOperator::L_BRACKET,	"(",		1},
	{KLParser::KLParserOperator::R_BRACKET,	")",		1}
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
	{KLParser::KLParserFunction::LN,		"ln"}
};

KLParser::KLParserToken::KLParserToken(void)
: LastError(NO_ERROR) {}

KLParser::KLParserToken::~KLParserToken(void) {}

template<typename Type>
Type* KLParser::KLParserToken::Instance(void)
{
	return dynamic_cast<Type*>(this);
}

KLParser::KLParserValue::KLParserValue(const char* Token)
{
	Value = atof(Token);
}

KLParser::KLParserValue::KLParserValue(double Token)
{
	Value = Token;
}

KLParser::KLParserValue::~KLParserValue(void) {}

unsigned KLParser::KLParserToken::GetPriority(void) const
{
	return 100;
}

KLParser::ERROR KLParser::KLParserToken::GetError(void) const
{
	return LastError;
}

KLString KLParser::KLParserValue::GetToken(void) const
{
	return KLString(Value);
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

KLString KLParser::KLParserOperator::GetToken(void) const
{
	return KLString(Operators[Operator].Token);
}

unsigned KLParser::KLParserOperator::GetPriority(void) const
{
	return Operators[Operator].Priority;
}

double KLParser::KLParserOperator::GetValue(KLList<double>& Values) const
{
	LastError = NO_ERROR;

	double ParamA = 0;
	double ParamB = 0;

	if (Operator != NOT)
	{
		if (Values.Size() < 2) LastError = NOT_ENOUGH_PARAMETERS;
		else
		{
			ParamB = Values.Pop();
			ParamA = Values.Pop();
		}
	}
	else
	{
		if (Values.Size() < 1) LastError = NOT_ENOUGH_PARAMETERS;
		else
		{
			ParamA = Values.Pop();
		}
	}

	if (LastError != NO_ERROR) return 0;

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
			return ((int) ParamA) % ((int) ParamB);
		case POW:
			return pow(ParamB, ParamA);

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
		case NOT:
			return !ParamA;

		default:
			return 0;
	}
}

KLParser::KLParserOperator::OPERATOR KLParser::KLParserOperator::GetOperator(void) const
{
	return Operator;
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

KLString KLParser::KLParserFunction::GetToken(void) const
{
	return KLString(Functions[Function].Token);
}

double KLParser::KLParserFunction::GetValue(KLList<double>& Values) const
{
	LastError = NO_ERROR;

	double ParamA = 0;

	if (Values.Size() < 1) LastError = NOT_ENOUGH_PARAMETERS;
	else
	{
		ParamA = Values.Pop();
	}

	if (LastError != NO_ERROR) return 0;

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
			return 0;
	}
}

KLParser::KLParserFunction::FUNCTION KLParser::KLParserFunction::GetFunction(void) const
{
	return Function;
}

KLParser::KLParser(unsigned short Buffer)
: BufferSize(Buffer), LastValue(0.0), LastError(NO_ERROR) {}

bool KLParser::GetTokens(KLList<KLParserToken*>& Tokens, const KLString& Code)
{
	if (Code.Count('(') != Code.Count(')'))
	{
		LastError = BRACKETS_NOT_EQUAL;

		return false;
	}

	KLList<KLParserToken*> Operators;

	bool functionExpected = false;
	char Buffer[BufferSize];
	int i = 0, Pos = 0;

	while (i = 0, Pos < Code.Size())
	{
		while (Pos < Code.Size() && isspace(Code[Pos])) Pos++;

		if (!Code[Pos]) break;

		if (isdigit(Code[Pos]))
		{
			do Buffer[i++] = Code[Pos++];
			while (isdigit(Code[Pos]));

			Buffer[i] = 0;

			Tokens << new KLParserValue(Buffer);
		}
		else if (isalpha(Code[Pos]))
		{
			do Buffer[i++] = Code[Pos++];
			while (isalpha(Code[Pos]));

			Buffer[i] = 0;

			KLParserToken* Function = new KLParserFunction(Buffer);

			if (Function->GetError())
			{
				LastError = Function->GetError();

				delete Function;

				return false;
			}

			Operators << Function;

			functionExpected = true;
		}
		else
		{
			if (functionExpected && Code[Pos] != '(')
			{
				LastError = EXPECTED_BRACKET;

				return false;
			}
			else
			{
				functionExpected = false;
			}

			switch (Code[Pos])
			{
				case ',':
					Pos++;
				break;
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
						    Token->Instance<KLParserOperator>())
						{
							if (Operator->GetOperator() == KLParserOperator::L_BRACKET)
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
					do Buffer[i++] = Code[Pos++];
					while (Code[Pos] &&
						  !isspace(Code[Pos]) &&
						  !isdigit(Code[Pos]) &&
						  !isalpha(Code[Pos]) &&
						  Code[Pos] != '(' &&
						  Code[Pos] != ')' &&
						  Code[Pos] != ',');

					Buffer[i] = 0;

					KLParserToken* Operator = new KLParserOperator(Buffer);

					if (Operator->GetError())
					{
						LastError = Operator->GetError();

						delete Operator;

						return false;
					}

					while (Operators.Size() &&
						  Operator->GetPriority() <= Operators[-1]->GetPriority())
					{
						Tokens << Operators.Pop();
					}

					Operators << Operator;
				}
			}
		}
	}

	while (Operators.Size()) Tokens << Operators.Pop();

	return true;
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
		if (KLParserValue* Value = Token->Instance<KLParserValue>())
		{
			Values << Value->GetValue();
		}
		else if (KLParserOperator* Operator = Token->Instance<KLParserOperator>())
		{
			Values << Operator->GetValue(Values);
		}
		else if (KLParserFunction* Function = Token->Instance<KLParserFunction>())
		{
			Values << Function->GetValue(Values);
		}

		if ((LastError = Token->GetError()) != NO_ERROR) break;
	}

	if (LastError == NO_ERROR && Values.Size() == 1) LastValue = Values.Pop();

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
