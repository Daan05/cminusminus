#ifndef STATEMENTS_HPP
#define STATEMENTS_HPP

#include <memory>

#include "expression.hpp"

struct StmtVisitor
{
   public:
    virtual ~StmtVisitor() = default;
    virtual void visit_expr_stmt(struct ExprStmt const &stmt) = 0;
    virtual void visit_print_stmt(struct PrintStmt const &stmt) = 0;
    virtual void visit_var_stmt(struct VarStmt const &stmt) = 0;
};

struct Stmt
{
   public:
    virtual ~Stmt() = default;
    virtual void accept(StmtVisitor &visitor) const = 0;
};

struct ExprStmt : public Stmt
{
   public:
    ExprStmt(std::unique_ptr<Expr> expr);
    void accept(StmtVisitor &visitor) const override;

    std::unique_ptr<Expr> expr;
};

struct PrintStmt : public Stmt
{
   public:
    PrintStmt(std::unique_ptr<Expr> expr);
    void accept(StmtVisitor &visitor) const override;

    std::unique_ptr<Expr> expr;
};

struct VarStmt : public Stmt
{
   public:
    VarStmt(Token token, std::unique_ptr<Expr> expr);
    void accept(StmtVisitor &visitor) const override;

    Token token;
    std::unique_ptr<Expr> expr;
};

#endif
