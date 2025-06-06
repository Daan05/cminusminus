#include "analyzer.hpp"

#include "common/error.hpp"

void ExprAnalyzer::analyze(Expr &expr)
{
    switch (expr.kind)
    {
    case ExprType::Binary:
    {
        analyze(*expr.variant.binary.left);
        analyze(*expr.variant.binary.right);
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
        analyze(*expr.variant.assign.expr);
        break;
    }
    case ExprType::Unary:
    {
        analyze(*expr.variant.unary.expr);
        break;
    }
    case ExprType::Grouping:
    {
        analyze(*expr.variant.grouping.expr);
        break;
    }
    default:
    {
        error::unreachable();
        break;
    }
    }
}
