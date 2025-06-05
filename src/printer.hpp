#ifndef PRINTER_HPP
#define PRINTER_HPP

#include "common/expression.hpp"

#include <string>
#include <variant>

struct ExprPrinter
{
    std::string indent;

    std::string print(Expr const &expr)
    {
        return std::visit(
            [&](auto &&e) -> std::string { return visit_expr(e); }, expr.expr
        );
    }

    std::string visit_expr(BinaryExpr const &expr)
    {
        return parenthesize(expr.op.lexeme, expr.left, expr.right);
    }

    std::string visit_expr(UnaryExpr const &expr)
    {
        return parenthesize(expr.op.lexeme, expr.right);
    }

    std::string visit_expr(LiteralExpr const &expr)
    {
        return expr.token.lexeme;
    }

    std::string visit_expr(GroupingExpr const &expr)
    {
        return parenthesize("group", expr.expr);
    }

    std::string visit_expr(VarExpr const &expr)
    {
        return expr.var.token.lexeme;
    }

    std::string visit_expr(AssignExpr const &expr)
    {
        return parenthesize("assign " + expr.var.token.lexeme, expr.expr);
    }

    template <typename... Exprs>
    std::string parenthesize(std::string const &name, Exprs const &...exprs)
    {
        std::string result = "(" + name;
        ((result += " " + print(*exprs)), ...);
        result += ")";
        return result;
    }
};

#endif
