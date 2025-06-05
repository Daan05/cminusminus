#ifndef STATEMENTS_HPP
#define STATEMENTS_HPP

#include <memory>
#include <vector>

#include "expression.hpp"

struct StmtVisitor
{
   public:
    virtual ~StmtVisitor() = default;
    virtual void visit_expr_stmt(struct ExprStmt &stmt) = 0;
    virtual void visit_print_stmt(struct PrintStmt &stmt) = 0;
    virtual void visit_var_stmt(struct VarStmt &stmt) = 0;
    virtual void visit_block_stmt(struct BlockStmt &stmt) = 0;
    virtual void visit_if_stmt(struct IfStmt &stmt) = 0;
};

struct Stmt
{
   public:
    virtual ~Stmt() = default;
    virtual void accept(StmtVisitor &visitor) = 0;
};

struct ExprStmt : public Stmt
{
   public:
    ExprStmt(std::unique_ptr<Expr> expr);
    void accept(StmtVisitor &visitor) override;

    std::unique_ptr<Expr> expr;
};

struct PrintStmt : public Stmt
{
   public:
    PrintStmt(std::unique_ptr<Expr> expr);
    void accept(StmtVisitor &visitor) override;

    std::unique_ptr<Expr> expr;
};

struct VarStmt : public Stmt
{
   public:
    VarStmt(LocalVar var, std::unique_ptr<Expr> expr);
    void accept(StmtVisitor &visitor) override;

    LocalVar var;
    std::unique_ptr<Expr> expr;
};

struct BlockStmt : public Stmt
{
   public:
    BlockStmt(std::vector<std::unique_ptr<Stmt>> statements);
    void accept(StmtVisitor &visitor) override;

    std::vector<std::unique_ptr<Stmt>> statements;
};

struct IfStmt : public Stmt
{
   public:
    IfStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> then_branch, std::unique_ptr<Stmt> else_branch);
    void accept(StmtVisitor &visitor) override;

    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> then_branch;
    std::unique_ptr<Stmt> else_branch;
};

#endif
