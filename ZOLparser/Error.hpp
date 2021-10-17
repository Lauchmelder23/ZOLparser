#pragma once

#include <stdexcept>
#include <string>

class SyntaxError : public std::runtime_error
{
public:
	SyntaxError(unsigned int position) :
		std::runtime_error(std::string("Syntax error at position ") + std::to_string(position)), pos(position)
	{}

	unsigned int where()
	{
		return pos;
	}

private:
	unsigned int pos;
};