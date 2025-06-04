#include "visitors.hpp"

#include "common/error.hpp"
#include "common/expression.hpp"

#include <iostream>
#include <string>
#include <vector>

std::string ExprPrinter::print(Expr &expr)
{
    m_output.clear();
    m_output.str("");
    expr.accept(*this);
    return m_output.str();
}

void ExprPrinter::visit_binary_expr(BinaryExpr &expr)
{
    m_output << "(" << expr.m_op.lexeme << " ";
    expr.m_left->accept(*this);
    m_output << " ";
    expr.m_right->accept(*this);
    m_output << ")";
}

void ExprPrinter::visit_literal_expr(LiteralExpr &expr)
{
    m_output << expr.token.literal.to_string();
}

void ExprPrinter::visit_var_expr(VarExpr &expr)
{
    m_output << expr.var->token.lexeme;
}

void ExprPrinter::visit_assign_expr(AssignExpr &expr)
{
    m_output << expr.var.token.lexeme << " = ";
    expr.m_expr->accept(*this);
}

void ExprPrinter::visit_unary_expr(UnaryExpr &expr)
{
    m_output << "(" << expr.m_op.lexeme << " ";
    expr.m_right->accept(*this);
    m_output << ")";
}

void ExprPrinter::visit_grouping_expr(GroupingExpr &expr)
{
    m_output << "(group ";
    expr.m_expr->accept(*this);
    m_output << ")";
}

void ExprAnalyzer::analyze(Expr &expr) { expr.accept(*this); }

void ExprAnalyzer::visit_binary_expr(BinaryExpr &expr)
{
    expr.m_left->accept(*this);
    expr.m_right->accept(*this);
}

void ExprAnalyzer::visit_literal_expr(LiteralExpr &) {}

void ExprAnalyzer::visit_var_expr(VarExpr &expr)
{
    bool is_declared = false;
    for (int ix = m_vars.size() - 1; ix >= 0; --ix)
    {
        if (expr.var->token.lexeme == m_vars[ix].token.lexeme)
        {
            is_declared = true;
            expr.var->rbp_offset = (ix + 1) * 8;
            break;
        }
    }
    if (!is_declared)
    {
        error::report(
            expr.var->token.line,
            "Undeclared variable '" + expr.var->token.lexeme + "'"
        );
    }
}

void ExprAnalyzer::visit_assign_expr(AssignExpr &expr)
{
    bool is_declared = false;
    for (int ix = m_vars.size() - 1; ix >= 0; --ix)
    {
        if (expr.var.token.lexeme == m_vars[ix].token.lexeme)
        {
            is_declared = true;
            expr.var.rbp_offset = (ix + 1) * 8;
            break;
        }
    }
    if (!is_declared)
    {
        error::report(
            expr.var.token.line,
            "Undeclared variable '" + expr.var.token.lexeme + "'"
        );
    }
    expr.m_expr->accept(*this);
}

void ExprAnalyzer::visit_unary_expr(UnaryExpr &expr)
{
    expr.m_right->accept(*this);
}

void ExprAnalyzer::visit_grouping_expr(GroupingExpr &expr)
{
    expr.m_expr->accept(*this);
}

std::string ExprCodeGenerator::generate(Expr &expr)
{
    m_output.clear();
    m_output.str("");
    expr.accept(*this);
    return m_output.str();
}

void ExprCodeGenerator::visit_binary_expr(BinaryExpr &expr)
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

void ExprCodeGenerator::visit_literal_expr(LiteralExpr &expr)
{
    m_output << "\tpush " << expr.token.literal.to_string() << "\n";
}

void ExprCodeGenerator::visit_var_expr(VarExpr &expr)
{
    m_output << "\tmov rax, qword [rbp - " << expr.var->rbp_offset << "]\n";
    m_output << "\tpush rax\n";
}

void ExprCodeGenerator::visit_assign_expr(AssignExpr &expr)
{
    expr.m_expr->accept(*this);
    m_output << "\tpop rax\n";
    m_output << "\tmov qword [rbp - " << expr.var.rbp_offset << "], rax\n";
}

void ExprCodeGenerator::visit_unary_expr(UnaryExpr &expr)
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

void ExprCodeGenerator::visit_grouping_expr(GroupingExpr &expr)
{
    expr.m_expr->accept(*this);
}

std::string StmtPrinter::print(Stmt &stmt)
{
    m_output.clear();
    m_output.str("");
    stmt.accept(*this);
    return m_output.str();
}

void StmtPrinter::visit_print_stmt(PrintStmt &stmt)
{
    m_output << "PRINT: ";
    ExprPrinter exprPrinter;
    m_output << exprPrinter.print(*stmt.expr) << '\n';
}

void StmtPrinter::visit_expr_stmt(ExprStmt &stmt)
{
    m_output << "EXPR: ";
    ExprPrinter exprPrinter;
    m_output << exprPrinter.print(*stmt.expr) << '\n';
}

void StmtPrinter::visit_var_stmt(VarStmt &stmt)
{
    m_output << "VAR DECL: ";
    m_output << "let " << stmt.var.token.lexeme << " = ";
    ExprPrinter exprPrinter;
    m_output << exprPrinter.print(*stmt.expr) << '\n';
}

void StmtPrinter::visit_block_stmt(BlockStmt &stmt)
{
    m_output << "START BLOCK\n";

    StmtPrinter printer;
    for (auto &stmt : stmt.statements)
    {
        m_output << "\t" << printer.print(*stmt);
    }

    m_output << "END BLOCK\n";
}

std::vector<LocalVar> StmtAnalyzer::m_vars = {};
int StmtAnalyzer::m_scope_depth = 0;

void StmtAnalyzer::analyze(Stmt &stmt) { stmt.accept(*this); }

void StmtAnalyzer::visit_print_stmt(PrintStmt &stmt)
{
    ExprAnalyzer analyzer(m_vars, m_scope_depth);
    analyzer.analyze(*stmt.expr);
}

void StmtAnalyzer::visit_expr_stmt(ExprStmt &stmt)
{
    ExprAnalyzer analyzer(m_vars, m_scope_depth);
    analyzer.analyze(*stmt.expr);
}

void StmtAnalyzer::visit_var_stmt(VarStmt &stmt)
{
    for (int ix = m_vars.size() - 1; ix >= 0; --ix)
    {
        if (stmt.var.token.lexeme == m_vars[ix].token.lexeme &&
            m_vars[ix].scope_depth == m_scope_depth)
        {
            error::report(stmt.var.token.line, "Already defined variable");
        }
    }
    m_vars.push_back(stmt.var);
    stmt.var.rbp_offset = m_vars.size() * 8;
}

void StmtAnalyzer::visit_block_stmt(BlockStmt &stmt)
{
    m_scope_depth++;

    StmtAnalyzer analyzer;
    for (auto &stmt : stmt.statements)
    {
        analyzer.analyze(*stmt);
    }

    for (int ix = m_vars.size() - 1; ix >= 0; --ix)
    {
        if (m_vars[ix].scope_depth >= m_scope_depth)
        {
            m_vars.pop_back();
        }
    }

    m_scope_depth--;
}

std::vector<LocalVar> StmtCodeGenerator::m_vars = {};
int StmtCodeGenerator::m_scope_depth = 0;

std::string StmtCodeGenerator::generate(Stmt &stmt)
{
    m_output.clear();
    m_output.str("");
    stmt.accept(*this);
    return m_output.str();
}

void StmtCodeGenerator::visit_print_stmt(PrintStmt &stmt)
{
    ExprCodeGenerator exprCodeGenerator;
    m_output << exprCodeGenerator.generate(*stmt.expr);
    m_output << "\tmov rdi, fmt ; 1st argument (format string)\n";
    m_output << "\tpop rsi ; 2nd argument (integer to print)\n";
    m_output << "\txor eax, eax ; Clear RAX: required before calling variadic "
                "functions like printf\n";
    m_output << "\tcall printf\n";
}

void StmtCodeGenerator::visit_expr_stmt(ExprStmt &stmt)
{
    ExprCodeGenerator exprCodeGenerator;
    m_output << exprCodeGenerator.generate(*stmt.expr);
}

void StmtCodeGenerator::visit_var_stmt(VarStmt &stmt)
{
    ExprCodeGenerator exprCodeGenerator;
    m_output << exprCodeGenerator.generate(*stmt.expr);
    m_output << "\tpop rax\n";
    m_output << "\tmov qword [rbp - " << stmt.var.rbp_offset << "], rax\n";
}

void StmtCodeGenerator::visit_block_stmt(BlockStmt &stmt)
{
    StmtCodeGenerator generator;
    for (auto const &stmt : stmt.statements)
    {
        m_output << generator.generate(*stmt);
    }
}
