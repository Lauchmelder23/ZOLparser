#pragma once

#include <vector>
#include "Token.hpp"
#include "Expression.hpp"

#include "Error.hpp"

class Parser
{
public:
	Parser(const std::string& text);

	const Token* ScanToken();

	Expression* ParseExpression();
	Expression* ParseWedge();
	Expression* ParseUnary();
	Expression* ParsePrimary();

	bool Evaluate() const;
	unsigned Size() const;
	unsigned VarCount() const;

private:
	std::vector<Token> tokens;
	std::vector<Token>::const_iterator nextToken;

	Expression* expressionTree = nullptr;
};