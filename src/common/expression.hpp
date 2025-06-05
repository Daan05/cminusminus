#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <memory>

#include "token.hpp"

struct LocalVar
{
    LocalVar(Token token, int scope_depth)
        : token(token), scope_depth(scope_depth), rbp_offset(0) {};
    ~LocalVar() = default;

    Token token;
    int scope_depth;
    int rbp_offset;
};

struct Expr;

struct BinaryExpr
{
    size_t line;
    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;
};

struct LiteralExpr
{
    size_t line;
    Token token;
};

struct VarExpr
{
    size_t line;
    LocalVar var;
};

struct AssignExpr
{
    size_t line;
    LocalVar var;
    std::unique_ptr<Expr> expr;
};

struct UnaryExpr
{
    size_t line;
    Token op;
    std::unique_ptr<Expr> right;
};

struct GroupingExpr
{
    size_t line;
    std::unique_ptr<Expr> expr;
};

struct Expr
{
    using ExprVariant = std::variant<
        BinaryExpr, LiteralExpr, VarExpr, AssignExpr, UnaryExpr, GroupingExpr>;

    ExprVariant expr;

    template <typename T>
        requires std::constructible_from<ExprVariant, T>
    Expr(T &&v) : expr(std::forward<T>(v))
    {
    }
};

#endif
