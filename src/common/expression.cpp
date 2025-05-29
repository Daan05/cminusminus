#include "expression.hpp"

Expr::Expr(int line) : line(line) {}

BinaryExpr::BinaryExpr(Expr left, Token op, Expr right, int line)
    : Expr(line), left(left), op(op), right(right)
{
}

LiteralExpr::LiteralExpr(Literal literal, int line)
    : Expr(line), literal(literal)
{
}

UnaryExpr::UnaryExpr(Token op, Expr right, int line)
    : Expr(line), op(op), right(right)
{
}

GroupingExpr::GroupingExpr(Expr expr, int line) : Expr(line), expr(expr) {}
