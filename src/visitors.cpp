#include "visitors.hpp"

#include "common/error.hpp"
#include "common/expression.hpp"

#include <iostream>
#include <string>
#include <vector>

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
    m_output << expr.token.literal.to_string();
}

void ExprPrinter::visit_var_decl_expr(VarExpr const &expr)
{
    m_output << expr.var.token.lexeme;
}

void ExprPrinter::visit_assign_expr(AssignExpr const &expr)
{
    m_output << expr.var.token.lexeme << " = ";
    expr.m_expr->accept(*this);
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
        error::todo(
            "ExprCodeGenerator::visit_binary_expr(): add support for "
            "division"
        );
        break;
    default:
        error::unreachable();
        break;
    }
}

void ExprCodeGenerator::visit_literal_expr(LiteralExpr const &expr)
{
    m_output << "\tpush " << expr.token.literal.to_string() << "\n";
}

void ExprCodeGenerator::visit_var_decl_expr(VarExpr const &expr)
{
    m_output << "\tmov rax, qword [rbp - " << expr.var.rbp_offset << "]\n";
    m_output << "\tpush rax\n";
}

void ExprCodeGenerator::visit_assign_expr(AssignExpr const &expr)
{
    expr.m_expr->accept(*this);
    m_output << "\tpop rax\n";
    m_output << "\tmov qword [rbp - " << expr.var.rbp_offset << "], rax\n";
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
        error::todo("ExprCodeGenerator::visit_unary_expr: add bang");
        break;
    default:
        error::unreachable();
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

void StmtPrinter::visit_var_stmt(VarStmt const &stmt)
{
    m_output << "VAR DECL: ";
    m_output << "let " << stmt.var.token.lexeme << " = ";
    ExprPrinter exprPrinter;
    m_output << exprPrinter.print(*stmt.expr) << '\n';
}

void StmtPrinter::visit_block_stmt(BlockStmt const &stmt)
{
    m_output << "START BLOCK\n";

    StmtPrinter printer;
    for (auto const &stmt : stmt.statements)
    {
        m_output << printer.print(*stmt);
    }

    m_output << "END BLOCK\n";
}

std::vector<LocalVar> StmtAnalyzer::m_vars = {};
int StmtAnalyzer::m_scope_depth = 0;

void StmtAnalyzer::analyze(Stmt const &stmt) { stmt.accept(*this); }

void StmtAnalyzer::visit_print_stmt(PrintStmt const &stmt) { (void)stmt; }

void StmtAnalyzer::visit_expr_stmt(ExprStmt const &stmt) { (void)stmt; }

void StmtAnalyzer::visit_var_stmt(VarStmt const &stmt)
{
    m_vars.push_back(stmt.var);
    // stmt.var.rbp_offset = m_vars.size() * 8;

    std::cout << "var stmt ";
    for (auto const &var : m_vars)
    {
        std::cout << "[" << var.token.lexeme << ", " << var.scope_depth << ", "
                  << var.rbp_offset << "]";
    }
    std::cout << '\n';
}

void StmtAnalyzer::visit_block_stmt(BlockStmt const &stmt)
{
    m_scope_depth++;
    std::cout << "start scope ";
    for (auto const &var : m_vars)
    {
        std::cout << "[" << var.token.lexeme << ", " << var.scope_depth << ", "
                  << var.rbp_offset << "]";
    }
    std::cout << '\n';

    StmtAnalyzer analyzer;
    for (auto const &stmt : stmt.statements)
    {
        analyzer.analyze(*stmt);
    }

    m_scope_depth--;

    for (int ix = m_vars.size() - 1; ix >= 0; --ix)
    {
        if (m_vars[ix].scope_depth > m_scope_depth)
        {
            std::cout << "pop\n";
            m_vars.pop_back();
        }
    }
    std::cout << "end scope ";
    for (auto const &var : m_vars)
    {
        std::cout << "[" << var.token.lexeme << ", " << var.scope_depth << ", "
                  << var.rbp_offset << "]";
    }
    std::cout << '\n';
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
    m_output << "\txor eax, eax ; Clear RAX: required before calling variadic "
                "functions like printf\n";
    m_output << "\tcall printf\n";
}

void StmtCodeGenerator::visit_expr_stmt(ExprStmt const &stmt)
{
    ExprCodeGenerator exprCodeGenerator;
    m_output << exprCodeGenerator.generate(*stmt.expr);
}

void StmtCodeGenerator::visit_var_stmt(VarStmt const &stmt)
{
    ExprCodeGenerator exprCodeGenerator;
    m_output << exprCodeGenerator.generate(*stmt.expr);
    m_output << "\tpop rax\n";
    m_output << "\tmov qword [rbp - " << stmt.var.rbp_offset << "], rax\n";
}

void StmtCodeGenerator::visit_block_stmt(BlockStmt const &stmt)
{
    // TODO
    m_output << "\t; START BLOCK\n";

    StmtCodeGenerator generator;
    for (auto const &stmt : stmt.statements)
    {
        m_output << generator.generate(*stmt);
    }

    m_output << "\t; END BLOCK\n";
}
