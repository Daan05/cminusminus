#include "printer.hpp"

#include "common/error.hpp"

std::string ExprPrinter::print(const Expr &expr)
{
    std::ostringstream oss;

    switch (expr.kind)
    {
    case ExprType::Binary:
        oss << "(" << expr.variant.binary.op.lexeme << " "
            << print(*expr.variant.binary.left) << " "
            << print(*expr.variant.binary.right) << ")";
        break;
    case ExprType::Literal:
        oss << expr.variant.literal.token.lexeme;
        break;
    case ExprType::Var:
        oss << expr.variant.var.var.token.lexeme;
        break;
    case ExprType::Assign:
        oss << "(assign " << expr.variant.assign.var.token.lexeme << " "
            << print(*expr.variant.assign.expr) << ")";
        break;
    case ExprType::Unary:
        oss << "(" << expr.variant.unary.op.lexeme << " "
            << print(*expr.variant.unary.expr) << ")";
        break;
    case ExprType::Grouping:
        oss << "(group " << print(*expr.variant.grouping.expr) << ")";
        break;
    default:
        error::unreachable();
        break;
    }

    return oss.str();
}
