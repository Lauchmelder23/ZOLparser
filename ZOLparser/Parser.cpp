#include "Parser.hpp"
#include <regex>
#include <iostream>

Parser::Parser(const std::string& text)
{
	unsigned int pos = 0;
	for (std::string::const_iterator it = text.begin(); it != text.end(); it++)
	{
		TokenType type;
		std::string lexeme(1, *it);

		switch (*it)
		{
		case 'p':
		case 'q':
		case 'r':
		case 's':
			type = TokenType::Atom;
			break;

		case '~':
			type = TokenType::Unary;
			break;

		case '^':
		case 'v':
			type = TokenType::Binary;
			break;

		case '-':
			if (*(++it) == '>')
			{
				lexeme += *it;
				type = TokenType::Binary;
				break;
			}
			throw SyntaxError(std::distance(text.begin(), it));

		case '<':
			if (*(++it) == '-' && *(++it) == '>')
			{
				lexeme += "->";
				type = TokenType::Binary;
				break;
			}
			throw SyntaxError(std::distance(text.begin(), it));
			
		case '(':
		case ')':
			type = TokenType::Parenthesis;
			break;

		default:
			throw SyntaxError(std::distance(text.begin(), it));
		}

		tokens.emplace_back(Token{ type, lexeme, pos});
		pos++;
	}
	tokens.emplace_back(Token{ TokenType::EOL, "", pos });

	nextToken = tokens.begin();

	expressionTree = ParseExpression();
	if (nextToken->type != TokenType::EOL)
	{
		throw SyntaxError(nextToken->position);
	}
}

const Token* Parser::ScanToken()
{
	const Token* returnVal = &(*nextToken);

	if (nextToken->type != TokenType::EOL) {
		returnVal = &(*nextToken);
		nextToken++;
	}

	return returnVal;
}

Expression* Parser::ParseExpression()
{
	Expression* expr = ParseWedge();

	while (nextToken->lexeme == "->" || nextToken->lexeme == "<->")
	{
		const Token* t = ScanToken();
		Expression* right = ParseWedge();
		if (t->lexeme == "->")
		{
			expr = new ImplicationExpression(expr, right);
		}
		else
		{
			expr = new EquivalenceExpression(expr, right);
		}
	}

	return expr;
}

Expression* Parser::ParseWedge()
{
	Expression* expr = ParseUnary();

	while (nextToken->lexeme == "^" || nextToken->lexeme == "v")
	{
		const Token* t = ScanToken();
		Expression* right = ParseUnary();
		if (t->lexeme == "^")
		{
			expr = new ConjunctionExpression(expr, right);
		}
		else 
		{
			expr = new DisjunctionExpression(expr, right);
		}
	}

	return expr;
}

Expression* Parser::ParseUnary()
{
	Expression* expr = nullptr;

	if (nextToken->lexeme == "~")
	{
		ScanToken();
		expr = new NegationExpression(ParseUnary());
	}
	else
	{
		expr = ParsePrimary();
	}

	return expr;
}

Expression* Parser::ParsePrimary()
{
	Expression* expr = nullptr;

	if (nextToken->type == TokenType::Atom)
	{
		const Token* t = ScanToken();
		switch (t->lexeme[0])
		{
		case 'p':	expr = &IdentifierExpression::p; break;
		case 'q':	expr = &IdentifierExpression::q; break;
		case 'r':	expr = &IdentifierExpression::r; break;
		case 's':	expr = &IdentifierExpression::s; break;
		}
	}
	else if(nextToken->lexeme == "(")
	{
		ScanToken();
		expr = ParseExpression();
		if (ScanToken()->lexeme != ")")
		{
			throw SyntaxError(nextToken->position);
		}
	}
	else
	{
		throw SyntaxError(nextToken->position);
	}

	return expr;
}

bool Parser::Evaluate() const
{
	return expressionTree->Eval();
}

unsigned Parser::Size() const
{
	return expressionTree->Size();
}

unsigned Parser::VarCount() const
{
	return expressionTree->VarCount();
}
