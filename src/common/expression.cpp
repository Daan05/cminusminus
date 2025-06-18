#include "expression.hpp"
#include <utility>

LocalVar::LocalVar(Token token, int scope_depth)
    : token(token), scope_depth(scope_depth) {};

BinaryExpr::BinaryExpr(
    std::unique_ptr<Expr> left, Token &&op, std::unique_ptr<Expr> right
)
    : left(std::move(left)), op(std::move(op)), right(std::move(right))
{
}

LiteralExpr::LiteralExpr(Token &&token) : token(std::move(token)) {}

VarExpr::VarExpr(LocalVar &&var) : var(std::move(var)) {}

AssignExpr::AssignExpr(LocalVar &&var, std::unique_ptr<Expr> expr)
    : var(std::move(var)), expr(std::move(expr))
{
}

UnaryExpr::UnaryExpr(Token &&op, std::unique_ptr<Expr> expr)
    : op(std::move(op)), expr(std::move(expr))
{
}

GroupingExpr::GroupingExpr(std::unique_ptr<Expr> expr) : expr(std::move(expr))
{
}

CallExpr::CallExpr(
    std::unique_ptr<Expr> callee, std::vector<std::unique_ptr<Expr>> args
)
    : callee(std::move(callee)), args(std::move(args))
{
}

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

Expr::Expr(size_t line, CallExpr &&expr) : line(line), kind(ExprType::Call)
{
    new (&variant.grouping) CallExpr(std::move(expr));
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
    case ExprType::Call:
        variant.call.~CallExpr();
    }
}
