#pragma once

class Expression
{
public:
	virtual bool Eval() const = 0;
	virtual unsigned int Size() const = 0;
	virtual unsigned int VarCount() const = 0;
};

class IdentifierExpression : public Expression
{
public:
	static IdentifierExpression p;
	static IdentifierExpression q;
	static IdentifierExpression r;
	static IdentifierExpression s;

public:
	void SetTruthValue(bool value) 
	{ 
		truthValue = value; 
	}

	bool Eval() const override 
	{ 
		return truthValue; 
	}

	unsigned int Size() const override { return 1; }
	unsigned int VarCount() const override {return 1; }

private:
	IdentifierExpression() :
		truthValue(false)
	{ }

private:
	bool truthValue;
};


class NegationExpression : public Expression
{
public:
	NegationExpression(const Expression* operand) :
		operand(operand)
	{

	}

	bool Eval() const override
	{
		return !(operand->Eval());
	}

	unsigned int Size() const override { return 1 + operand->Size(); }
	unsigned int VarCount() const override { return operand->VarCount(); }

private:
	const Expression* operand;
};


class BinaryExpression : public Expression
{
public:
	BinaryExpression(const Expression* left, const Expression* right) :
		left(left), right(right)
	{

	}

	unsigned int Size() const override { return 1 + left->Size() + right->Size(); }
	unsigned int VarCount() const override { return left->VarCount() + right->VarCount(); }

protected:
	const Expression* left;
	const Expression* right;
};


class ConjunctionExpression : public BinaryExpression
{
public:
	using BinaryExpression::BinaryExpression;

	bool Eval() const override
	{
		return left->Eval() && right->Eval();
	}
};

class DisjunctionExpression : public BinaryExpression
{
public:
	using BinaryExpression::BinaryExpression;

	bool Eval() const override
	{
		return left->Eval() || right->Eval();
	}
};

class ImplicationExpression : public BinaryExpression
{
public:
	using BinaryExpression::BinaryExpression;

	bool Eval() const override
	{
		return !(left->Eval()) || right->Eval();
	}
};

class EquivalenceExpression : public BinaryExpression
{
public:
	using BinaryExpression::BinaryExpression;

	bool Eval() const override
	{
		return left->Eval() == right->Eval();
	}
};