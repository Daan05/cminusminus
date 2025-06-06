#include "analyzer.hpp"

#include "common/error.hpp"

void Analyzer::analyze_expr(Expr &expr)
{
    switch (expr.kind)
    {
    case ExprType::Binary:
    {
        analyze_expr(*expr.variant.binary.left);
        analyze_expr(*expr.variant.binary.right);
        break;
    }
    case ExprType::Literal:
    {
        break;
    }
    case ExprType::Var:
    {
        VarExpr &var_expr = expr.variant.var;
        bool is_declared = false;
        for (int ix = m_vars.size() - 1; ix >= 0; --ix)
        {
            if (var_expr.var.token.lexeme == m_vars[ix].token.lexeme)
            {
                is_declared = true;
                var_expr.var.rbp_offset = (ix + 1) * 8;
                break;
            }
        }
        if (!is_declared)
        {
            error::report(
                var_expr.var.token.line,
                "Undeclared variable '" + var_expr.var.token.lexeme + "'"
            );
        }
        break;
    }
    case ExprType::Assign:
    {
        AssignExpr &assign_expr = expr.variant.assign;
        bool is_declared = false;
        for (int ix = m_vars.size() - 1; ix >= 0; --ix)
        {
            if (assign_expr.var.token.lexeme == m_vars[ix].token.lexeme)
            {
                is_declared = true;
                assign_expr.var.rbp_offset = (ix + 1) * 8;
                break;
            }
        }
        if (!is_declared)
        {
            error::report(
                assign_expr.var.token.line,
                "Undeclared variable '" + assign_expr.var.token.lexeme + "'"
            );
        }
        analyze_expr(*expr.variant.assign.expr);
        break;
    }
    case ExprType::Unary:
    {
        analyze_expr(*expr.variant.unary.expr);
        break;
    }
    case ExprType::Grouping:
    {
        analyze_expr(*expr.variant.grouping.expr);
        break;
    }
    default:
    {
        error::unreachable();
        break;
    }
    }
}

void Analyzer::analyze_stmt(Stmt &stmt)
{
    switch (stmt.kind)
    {
    case StmtType::Expr:
        analyze_expr(*stmt.variant.expr.expr);
        break;
    case StmtType::Print:
        analyze_expr(*stmt.variant.print.expr);
        break;
    case StmtType::Var:
        for (int ix = m_vars.size() - 1; ix >= 0; --ix)
        {
            if (stmt.variant.var.var.token.lexeme == m_vars[ix].token.lexeme &&
                m_vars[ix].scope_depth == m_scope_depth)
            {
                error::report(
                    stmt.variant.var.var.token.line, "Already defined variable"
                );
            }
        }
        m_vars.push_back(stmt.variant.var.var);
        if (m_vars.size() > 256)
        {
            error::report(
                stmt.variant.var.var.token.line,
                "Limit of 256 local vars has been exceeded"
            );
        }
        stmt.variant.var.var.rbp_offset = m_vars.size() * 8;
        break;
    case StmtType::Block:
        m_scope_depth++;
        for (auto &stmt : stmt.variant.block.statements)
        {
            analyze_stmt(*stmt);
        }
        while (!m_vars.empty() && m_vars.back().scope_depth >= m_scope_depth)
        {
            m_vars.pop_back();
        }
        m_scope_depth--;
        break;
    case StmtType::If:
        analyze_expr(*stmt.variant.if_.condition);

        analyze_stmt(*stmt.variant.if_.then_branch);
        analyze_stmt(*stmt.variant.if_.else_branch);
        break;
    case StmtType::While:
        analyze_expr(*stmt.variant.while_.condition);
        analyze_stmt(*stmt.variant.while_.body);
        break;
    default:
        error::unreachable();
        break;
    }
}
