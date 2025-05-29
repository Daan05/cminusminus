#include "expression.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "token.hpp"

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

void BinaryExpr::accept(Visitor &visitor) const { visitor.visitBinary(*this); }

LiteralExpr::LiteralExpr(Literal literal, int line)
    : Expr(line), m_literal(std::move(literal))
{
}

void LiteralExpr::accept(Visitor &visitor) const
{
    visitor.visitLiteral(*this);
}

UnaryExpr::UnaryExpr(Token op, std::unique_ptr<Expr> right, int line)
    : Expr(line), m_op(std::move(op)), m_right(std::move(right))
{
}

void UnaryExpr::accept(Visitor &visitor) const { visitor.visitUnary(*this); }

GroupingExpr::GroupingExpr(std::unique_ptr<Expr> expr, int line)
    : Expr(line), m_expr(std::move(expr))
{
}

void GroupingExpr::accept(Visitor &visitor) const
{
    visitor.visitGrouping(*this);
}

void ASTPrinter::print(Expr const &expr)
{
    m_output.clear();
    expr.accept(*this);
    std::cout << m_output << std::endl;
}

void ASTPrinter::visitBinary(BinaryExpr const &expr)
{
    m_output += "(" + expr.m_op.lexeme + " ";
    expr.m_left->accept(*this);
    m_output += " ";
    expr.m_right->accept(*this);
    m_output += ")";
}

void ASTPrinter::visitLiteral(LiteralExpr const &expr)
{
    m_output += expr.m_literal.to_string();
}

void ASTPrinter::visitUnary(UnaryExpr const &expr)
{
    m_output += "(" + expr.m_op.lexeme + " ";
    expr.m_right->accept(*this);
    m_output += ")";
}

void ASTPrinter::visitGrouping(GroupingExpr const &expr)
{
    m_output += "(group ";
    expr.m_expr->accept(*this);
    m_output += ")";
}
