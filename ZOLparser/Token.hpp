#pragma once

#include <string>

enum class TokenType
{
	Atom,
	Parenthesis,
	Unary,
	Binary,
	EOL
};

struct Token
{
	TokenType type;
	std::string lexeme;
	unsigned int position;
};