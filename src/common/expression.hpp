#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <variant>

#include "literal.hpp"

// add precedence later on
enum class BinaryOperator
{
    Add,
    Sub,
    Mul,
    Div,

    // Equal,
    // NotEqual,
    // etc...
};

class Expr
{
   public:
    virtual ~Expr() = default;
    int getLine() const { return line; }

   protected:
    Expr(int line);

   private:
    int line;
};

class BinaryExpr : public Expr
{
    BinaryExpr(Expr left, BinaryOperator op, Expr right, int line);

    Expr left;
    BinaryOperator op;
    Expr right;
};

class LiteralExpr : public Expr
{
    LiteralExpr(Literal literal, int line);

    Literal literal;
};

using ExprType = std::variant<LiteralExpr, BinaryExpr>;

#endif
