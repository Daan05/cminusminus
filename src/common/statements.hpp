#ifndef STATEMENTS_HPP
#define STATEMENTS_HPP

#include <memory>

#include "expression.hpp"

class StmtVisitor
{
   public:
    virtual ~StmtVisitor() = default;
    virtual void visit_expr_stmt(class ExprStmt const &stmt) = 0;
    virtual void visit_print_stmt(class PrintStmt const &stmt) = 0;
};

class Stmt
{
   public:
    virtual ~Stmt() = default;
    virtual void accept(StmtVisitor &visitor) const = 0;
};

class ExprStmt : public Stmt
{
   public:
    ExprStmt(std::unique_ptr<Expr> expr);
    void accept(StmtVisitor &visitor) const override;

    std::unique_ptr<Expr> expr;
};

class PrintStmt : public Stmt
{
   public:
    PrintStmt(std::unique_ptr<Expr> expr);
    void accept(StmtVisitor &visitor) const override;

    std::unique_ptr<Expr> expr;
};

#endif
