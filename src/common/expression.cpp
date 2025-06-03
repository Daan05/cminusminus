#include "expression.hpp"

#include <iostream>
#include <memory>

#include "token.hpp"

Var::Var(Token token, int offset) : token(token), rbp_offset(offset)
{
    // std::cout << offset << '\n';
};

Expr::Expr(int line) : m_line(line) {}

BinaryExpr::BinaryExpr(
    std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right, int line
)
    : Expr(line),
      m_left(std::move(left)),
      m_op(std::move(op)),
      m_right(std::move(right))
{
}

void BinaryExpr::accept(ExprVisitor &visitor) const
{
    visitor.visit_binary_expr(*this);
}

LiteralExpr::LiteralExpr(Token token, int line)
    : Expr(line), token(std::move(token))
{
}

void LiteralExpr::accept(ExprVisitor &visitor) const
{
    visitor.visit_literal_expr(*this);
}

VarExpr::VarExpr(Token token, int offset, int line)
    : Expr(line), var(std::move(Var(std::move(token), offset)))
{
}

void VarExpr::accept(ExprVisitor &visitor) const
{
    visitor.visit_var_decl_expr(*this);
}

AssignExpr::AssignExpr(Var var, std::unique_ptr<Expr> expr, int line)
    : Expr(line), var(std::move(var)), m_expr(std::move(expr))
{
}

void AssignExpr::accept(ExprVisitor &visitor) const
{
    visitor.visit_assign_expr(*this);
}

UnaryExpr::UnaryExpr(Token op, std::unique_ptr<Expr> right, int line)
    : Expr(line), m_op(std::move(op)), m_right(std::move(right))
{
}

void UnaryExpr::accept(ExprVisitor &visitor) const
{
    visitor.visit_unary_expr(*this);
}

GroupingExpr::GroupingExpr(std::unique_ptr<Expr> expr, int line)
    : Expr(line), m_expr(std::move(expr))
{
}

void GroupingExpr::accept(ExprVisitor &visitor) const
{
    visitor.visit_grouping_expr(*this);
}
