#include "printer.hpp"

#include <sstream>

#include "common/error.hpp"

std::string Printer::print_expr(const Expr &expr)
{
    std::ostringstream oss;

    switch (expr.kind)
    {
    case ExprType::Binary:
        oss << "(" << expr.variant.binary.op.lexeme << " "
            << print_expr(*expr.variant.binary.left) << " "
            << print_expr(*expr.variant.binary.right) << ")";
        break;
    case ExprType::Literal:
        oss << expr.variant.literal.token.lexeme;
        break;
    case ExprType::Var:
        oss << expr.variant.var.var.token.lexeme;
        break;
    case ExprType::Assign:
        oss << "(= " << expr.variant.assign.var.token.lexeme << " "
            << print_expr(*expr.variant.assign.expr) << ")";
        break;
    case ExprType::Unary:
        oss << "(" << expr.variant.unary.op.lexeme << " "
            << print_expr(*expr.variant.unary.expr) << ")";
        break;
    case ExprType::Grouping:
        oss << "(group " << print_expr(*expr.variant.grouping.expr) << ")";
        break;
    default:
        error::unreachable();
        break;
    }

    return oss.str();
}
