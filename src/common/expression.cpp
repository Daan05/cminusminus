#include "expression.hpp"

#include <memory>
#include <stdexcept>
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

std::string ASTPrinter::print(Expr const &expr)
{
    m_output.clear();
    expr.accept(*this);
    return m_output.str();
}

void ASTPrinter::visitBinary(BinaryExpr const &expr)
{
    m_output << "(" << expr.m_op.lexeme << " ";
    expr.m_left->accept(*this);
    m_output << " ";
    expr.m_right->accept(*this);
    m_output << ")";
}

void ASTPrinter::visitLiteral(LiteralExpr const &expr)
{
    m_output << expr.m_literal.to_string();
}

void ASTPrinter::visitUnary(UnaryExpr const &expr)
{
    m_output << "(" << expr.m_op.lexeme << " ";
    expr.m_right->accept(*this);
    m_output << ")";
}

void ASTPrinter::visitGrouping(GroupingExpr const &expr)
{
    m_output << "(group ";
    expr.m_expr->accept(*this);
    m_output << ")";
}

std::string ASTCodeGenerator::generate(Expr const &expr)
{
    m_output.clear();
    expr.accept(*this);
    return m_output.str();
}

void ASTCodeGenerator::visitBinary(BinaryExpr const &expr)
{
    expr.m_left->accept(*this);
    expr.m_right->accept(*this);

    m_output << "\tpop rax\n";
    m_output << "\tpop rcx\n";
    switch (expr.m_op.kind)
    {
    case TokenType::Plus:
        m_output << "\tadd rax, rcx\n";
        m_output << "\tpush rax\n";
        break;
    case TokenType::Minus:
        m_output << "\tsub rcx, rax\n";
        m_output << "\tpush rcx\n";
        break;
    case TokenType::Star:
        m_output << "\timul rax, rcx\n";
        m_output << "\tpush rax\n";
        break;
    case TokenType::Slash:
        // later
        break;
    default:
        throw std::runtime_error(
            "unreachable code: ASTCodeGenerator::visitBinary()"
        );
        break;
    }
}

void ASTCodeGenerator::visitLiteral(LiteralExpr const &expr)
{
    m_output << "\tpush " + expr.m_literal.to_string() + "\n";
}

void ASTCodeGenerator::visitUnary(UnaryExpr const &expr)
{
    expr.m_right->accept(*this);
    switch (expr.m_op.kind)
    {
    case TokenType::Minus:
        m_output << "\tpop rax\n";
        m_output << "\tneg rax\n";
        m_output << "\tpush rax\n";
        break;
    case TokenType::Bang:
        // later
        break;
    default:
        throw std::runtime_error(
            "unreachable code: ASTCodeGenerator::visitUnary()"
        );
        break;
    }
}

void ASTCodeGenerator::visitGrouping(GroupingExpr const &expr)
{
    expr.m_expr->accept(*this);
}
