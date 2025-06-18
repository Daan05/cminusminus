#ifndef STATEMENTS_HPP
#define STATEMENTS_HPP

#include <memory>
#include <vector>

#include "token.hpp"
#include "expression.hpp"

enum class StmtType
{
    Expr,
    Print,
    Var,
    Block,
    If,
    While,
    Func,
};

struct Stmt;

struct ExprStmt
{
    ExprStmt(std::unique_ptr<Expr> expr);
    ExprStmt(ExprStmt &&stmt) = default;
    ~ExprStmt() = default;

    std::unique_ptr<Expr> expr;
};

struct PrintStmt
{
    PrintStmt(std::unique_ptr<Expr> expr);
    PrintStmt(PrintStmt &&expr) = default;
    ~PrintStmt() = default;

    std::unique_ptr<Expr> expr;
};

struct VarStmt
{
    VarStmt(LocalVar var, std::unique_ptr<Expr> expr);
    VarStmt(VarStmt &&stmt) = default;
    ~VarStmt() = default;

    LocalVar var;
    std::unique_ptr<Expr> expr;
};

struct BlockStmt
{
    BlockStmt(std::vector<std::unique_ptr<Stmt>> statements);
    BlockStmt(BlockStmt &&stmt) = default;
    ~BlockStmt() = default;

    std::vector<std::unique_ptr<Stmt>> statements;
};

struct IfStmt
{
    IfStmt(
        std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> then_branch,
        std::unique_ptr<Stmt> else_branch
    );
    IfStmt(IfStmt &&stmt) = default;
    ~IfStmt() = default;

    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> then_branch;
    std::unique_ptr<Stmt> else_branch;
};

struct WhileStmt
{
    WhileStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body);
    WhileStmt(WhileStmt &&stmt) = default;
    ~WhileStmt() = default;

    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> body;
};

struct FuncStmt
{
    FuncStmt(
        Token &&name, std::vector<Token> params,
        std::unique_ptr<Stmt> body
    );
    FuncStmt(FuncStmt &&stmt) = default;
    ~FuncStmt() = default;

    Token name;
    std::vector<Token> params;
    std::unique_ptr<Stmt> body;
};

struct Stmt
{
    Stmt(size_t line, ExprStmt &&expr);
    Stmt(size_t line, PrintStmt &&expr);
    Stmt(size_t line, VarStmt &&expr);
    Stmt(size_t line, BlockStmt &&expr);
    Stmt(size_t line, IfStmt &&expr);
    Stmt(size_t line, WhileStmt &&expr);
    Stmt(size_t line, FuncStmt &&expr);
    ~Stmt();

    size_t line;

    StmtType kind;
    union Variant
    {
        ExprStmt expr;
        PrintStmt print;
        VarStmt var;
        BlockStmt block;
        IfStmt if_;
        WhileStmt while_;
        FuncStmt func;

        Variant() {}
        ~Variant() {}
    } variant;
};

#endif
