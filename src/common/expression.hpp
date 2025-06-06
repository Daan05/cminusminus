#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <cstddef>
#include <memory>

#include "token.hpp"

struct LocalVar
{
    LocalVar(Token token, int scope_depth);
    ~LocalVar() = default;

    Token token;
    int scope_depth;
    int rbp_offset;
};

enum class ExprType
{
    Binary,
    Literal,
    Var,
    Assign,
    Unary,
    Grouping,
};

struct Expr;

struct BinaryExpr
{
    BinaryExpr(
        std::unique_ptr<Expr> left, Token &&op, std::unique_ptr<Expr> right
    );
    BinaryExpr(BinaryExpr &&expr) = default;
    ~BinaryExpr() = default;

    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;
};

struct LiteralExpr
{
    LiteralExpr(Token &&token);
    LiteralExpr(LiteralExpr &&expr) = default;
    ~LiteralExpr() = default;

    Token token;
};

struct VarExpr
{
    VarExpr(LocalVar &&var);
    VarExpr(VarExpr &&expr) = default;
    ~VarExpr() = default;

    LocalVar var;
};

struct AssignExpr
{
    AssignExpr(LocalVar &&var, std::unique_ptr<Expr> expr);
    AssignExpr(AssignExpr &&expr) = default;
    ~AssignExpr() = default;

    LocalVar var;
    std::unique_ptr<Expr> expr;
};

struct UnaryExpr
{
    UnaryExpr(Token &&op, std::unique_ptr<Expr> expr);
    UnaryExpr(UnaryExpr &&expr) = default;
    ~UnaryExpr() = default;

    Token op;
    std::unique_ptr<Expr> expr;
};

struct GroupingExpr
{
    GroupingExpr(std::unique_ptr<Expr> expr);
    GroupingExpr(GroupingExpr &&expr) = default;
    ~GroupingExpr() = default;

    std::unique_ptr<Expr> expr;
};

struct Expr
{
    Expr(size_t line, BinaryExpr &&expr);
    Expr(size_t line, LiteralExpr &&expr);
    Expr(size_t line, VarExpr &&expr);
    Expr(size_t line, AssignExpr &&expr);
    Expr(size_t line, UnaryExpr &&expr);
    Expr(size_t line, GroupingExpr &&expr);
    ~Expr();

    size_t line;

    ExprType kind;
    union Variant
    {
        BinaryExpr binary;
        LiteralExpr literal;
        VarExpr var;
        AssignExpr assign;
        UnaryExpr unary;
        GroupingExpr grouping;

        Variant() {}
        ~Variant() {}
    } variant;
};

#endif
