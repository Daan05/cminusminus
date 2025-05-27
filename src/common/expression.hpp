#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <variant>

#include "literal.hpp"
#include "token.hpp"

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
    BinaryExpr(Expr left, Token op, Expr right, int line);

    Expr left;
    Token op;
    Expr right;
};

class LiteralExpr : public Expr
{
    LiteralExpr(Literal literal, int line);

    Literal literal;
};

using ExpressionType = std::variant<Literal, BinaryExpr>;

#endif
