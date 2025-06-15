#include "irinstructions.hpp"
#include <string>

AssignIRInstr::AssignIRInstr(std::string dst, std::string src)
    : dst(dst), src(src)
{
}

BinaryOpIRInstr::BinaryOpIRInstr(
    std::string dst, std::string left, std::string op, std::string right
)
    : dst(dst), left(left), op(op), right(right)
{
}

// etc

Expr::Expr(size_t line, BinaryExpr &&expr) : line(line), kind(ExprType::Binary)
{
    new (&variant.binary) BinaryExpr(std::move(expr));
}

Expr::Expr(size_t line, LiteralExpr &&expr)
    : line(line), kind(ExprType::Literal)
{
    new (&variant.literal) LiteralExpr(std::move(expr));
}

Expr::Expr(size_t line, VarExpr &&expr) : line(line), kind(ExprType::Var)
{
    new (&variant.var) VarExpr(std::move(expr));
}

Expr::Expr(size_t line, AssignExpr &&expr) : line(line), kind(ExprType::Assign)
{
    new (&variant.assign) AssignExpr(std::move(expr));
}

Expr::Expr(size_t line, UnaryExpr &&expr) : line(line), kind(ExprType::Unary)
{
    new (&variant.unary) UnaryExpr(std::move(expr));
}

Expr::Expr(size_t line, GroupingExpr &&expr)
    : line(line), kind(ExprType::Grouping)
{
    new (&variant.grouping) GroupingExpr(std::move(expr));
}

Expr::~Expr()
{
    switch (kind)
    {
    case ExprType::Binary:
        variant.binary.~BinaryExpr();
        break;
    case ExprType::Literal:
        variant.literal.~LiteralExpr();
        break;
    case ExprType::Var:
        variant.var.~VarExpr();
        break;
    case ExprType::Assign:
        variant.assign.~AssignExpr();
        break;
    case ExprType::Unary:
        variant.unary.~UnaryExpr();
        break;
    case ExprType::Grouping:
        variant.grouping.~GroupingExpr();
        break;
    }
}
