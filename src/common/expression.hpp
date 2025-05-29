#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <variant>

#include "literal.hpp"
#include "token.hpp"

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
   public:
    BinaryExpr(Expr left, Token op, Expr right, int line);

   private:
    Expr left;
    Token op;
    Expr right;
};

class LiteralExpr : public Expr
{
   public:
    LiteralExpr(Literal literal, int line);

   private:
    Literal literal;
};

class UnaryExpr : public Expr
{
   public:
    UnaryExpr(Token op, Expr right, int line);

   private:
    Token op;
    Expr right;
};

class GroupingExpr : public Expr
{
   public:
    GroupingExpr(Expr expr, int line);

   private:
    Expr expr;
};

#endif
