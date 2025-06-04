#include "expression.hpp"

#include <memory>

#include "token.hpp"

LocalVar::LocalVar(Token token, int scope_depth)
    : token(token), scope_depth(scope_depth), rbp_offset(0) {};

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

void BinaryExpr::accept(ExprVisitor &visitor)
{
    visitor.visit_binary_expr(*this);
}

LiteralExpr::LiteralExpr(Token token, int line)
    : Expr(line), token(std::move(token))
{
}

void LiteralExpr::accept(ExprVisitor &visitor)
{
    visitor.visit_literal_expr(*this);
}

VarExpr::VarExpr(Token token, int scope_depth, int line)
    : Expr(line),
      var(std::make_unique<LocalVar>(LocalVar(std::move(token), scope_depth)))
{
}

void VarExpr::accept(ExprVisitor &visitor) { visitor.visit_var_expr(*this); }

AssignExpr::AssignExpr(LocalVar var, std::unique_ptr<Expr> expr, int line)
    : Expr(line), var(std::move(var)), m_expr(std::move(expr))
{
}

void AssignExpr::accept(ExprVisitor &visitor)
{
    visitor.visit_assign_expr(*this);
}

UnaryExpr::UnaryExpr(Token op, std::unique_ptr<Expr> right, int line)
    : Expr(line), m_op(std::move(op)), m_right(std::move(right))
{
}

void UnaryExpr::accept(ExprVisitor &visitor)
{
    visitor.visit_unary_expr(*this);
}

GroupingExpr::GroupingExpr(std::unique_ptr<Expr> expr, int line)
    : Expr(line), m_expr(std::move(expr))
{
}

void GroupingExpr::accept(ExprVisitor &visitor)
{
    visitor.visit_grouping_expr(*this);
}
