#include <iostream>
#include "Parser.hpp"

void PrintHelp();

int main(int argc, char** argv)
{
	std::string text;
	Parser* parser;

	std::cout << "Zeroth order logic evaluator" << std::endl;
	std::cout << "Enter expression or ?: ";
	std::cin >> text;

	if (text == "?")
	{
		PrintHelp();
		return 0;
	}

	try
	{
		parser = new Parser(text);
	}
	catch (SyntaxError err)
	{
		std::cerr << err.what() << std::endl;
		std::cerr << text << std::endl;
		for (int i = 0; i < err.where(); i++)
			std::cerr << " ";
		std::cerr << "^" << std::endl;
		return 1;
	}

	unsigned int tableLength = sizeof("| p | q | r | s || ") + text.length() + 2;
	// Print table header
	std::cout << "+---+---+---+---++-" << std::string(text.length(), '-') << "-+" << std::endl;
	std::cout << "| p | q | r | s || " << text << " |" << std::endl;
	std::cout << "+---+---+---+---++-" << std::string(text.length(), '-') << "-+" << std::endl;

	for (int p = 0; p != 2; p++)
	{
		IdentifierExpression::p.SetTruthValue(p);
		for (int q = 0; q != 2; q++)
		{
			IdentifierExpression::q.SetTruthValue(q);
			for (int r = 0; r != 2; r++)
			{
				IdentifierExpression::r.SetTruthValue(r);
				for (int s = 0; s != 2; s++)
				{
					IdentifierExpression::s.SetTruthValue(s);

					std::cout << "| " << p << " | " << q << " | " << r << " | " << s << " || " << std::string(text.length() / 2, ' ') << parser->Evaluate() << std::string(text.length() - text.length() / 2, ' ') << "|" << std::endl;

				}
			}
		}
	}

	std::cout << "+---+---+---+---++-" << std::string(text.length(), '-') << "-+" << std::endl;

	std::cout << "Size     = " << parser->Size() << std::endl;
	std::cout << "VarCount = " << parser->VarCount() << std::endl;

	return 0;
}

void PrintHelp()
{
	std::cout << "This program creates truth value tables for zeroth order logic expressions with up to four atoms." << std::endl;
	std::cout << "Syntax:" << std::endl;
	std::cout << "   p,q,r,s -      Atom" << std::endl;
	std::cout << "   ~       -      Negation    (unary)" << std::endl;
	std::cout << "   ^       -      Conjunction (binary)" << std::endl;
	std::cout << "   v       -      Disjunction (binary)" << std::endl;
	std::cout << "   ->      -      Implication (binary)" << std::endl;
	std::cout << "   <->     -      Implication (binary)" << std::endl;
	std::cout << std::endl << "~ binds stronger than (^, v) which bind stronger than (->, <->)" << std::endl;
}
