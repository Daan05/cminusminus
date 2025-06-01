#include "visitors.hpp"

std::string ExprPrinter::print(Expr const &expr)
{
    m_output.clear();
    m_output.str("");
    expr.accept(*this);
    return m_output.str();
}

void ExprPrinter::visit_binary_expr(BinaryExpr const &expr)
{
    m_output << "(" << expr.m_op.lexeme << " ";
    expr.m_left->accept(*this);
    m_output << " ";
    expr.m_right->accept(*this);
    m_output << ")";
}

void ExprPrinter::visit_literal_expr(LiteralExpr const &expr)
{
    m_output << expr.m_literal.to_string();
}

void ExprPrinter::visit_unary_expr(UnaryExpr const &expr)
{
    m_output << "(" << expr.m_op.lexeme << " ";
    expr.m_right->accept(*this);
    m_output << ")";
}

void ExprPrinter::visit_grouping_expr(GroupingExpr const &expr)
{
    m_output << "(group ";
    expr.m_expr->accept(*this);
    m_output << ")";
}

std::string ExprCodeGenerator::generate(Expr const &expr)
{
    m_output.clear();
    m_output.str("");
    expr.accept(*this);
    return m_output.str();
}

void ExprCodeGenerator::visit_binary_expr(BinaryExpr const &expr)
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
        // TODO: add division
        break;
    default:
        throw std::runtime_error(
            "unreachable code: ASTCodeGenerator::visitBinary()"
        );
        break;
    }
}

void ExprCodeGenerator::visit_literal_expr(LiteralExpr const &expr)
{
    m_output << "\tpush " + expr.m_literal.to_string() + "\n";
}

void ExprCodeGenerator::visit_unary_expr(UnaryExpr const &expr)
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
        // TODO: add bang
        break;
    default:
        throw std::runtime_error(
            "unreachable code: ASTCodeGenerator::visitUnary()"
        );
        break;
    }
}

void ExprCodeGenerator::visit_grouping_expr(GroupingExpr const &expr)
{
    expr.m_expr->accept(*this);
}

std::string StmtPrinter::print(Stmt const &stmt)
{
    m_output.clear();
    m_output.str("");
    stmt.accept(*this);
    return m_output.str();
}

void StmtPrinter::visit_print_stmt(PrintStmt const &stmt)
{
    m_output << "PRINT: ";
    ExprPrinter exprPrinter;
    m_output << exprPrinter.print(*stmt.expr) << '\n';
}

void StmtPrinter::visit_expr_stmt(ExprStmt const &stmt)
{
    m_output << "EXPR: ";
    ExprPrinter exprPrinter;
    m_output << exprPrinter.print(*stmt.expr) << '\n';
}

std::string StmtCodeGenerator::generate(Stmt const &stmt)
{
    m_output.clear();
    m_output.str("");
    stmt.accept(*this);
    return m_output.str();
}

void StmtCodeGenerator::visit_print_stmt(PrintStmt const &stmt)
{
    ExprCodeGenerator exprCodeGenerator;
    m_output << exprCodeGenerator.generate(*stmt.expr);
    m_output << "\tmov rdi, fmt ; 1st argument (format string)\n";
	m_output << "\tpop rsi ; 2nd argument (integer to print)\n";
	m_output << "\txor eax, eax ; Clear RAX: required before calling variadic functions like printf\n";
	m_output << "\tcall printf\n";
}

void StmtCodeGenerator::visit_expr_stmt(ExprStmt const &stmt)
{
    ExprCodeGenerator exprCodeGenerator;
    m_output << exprCodeGenerator.generate(*stmt.expr);
}
