#include "statements.hpp"

#include <memory>
#include <utility>

ExprStmt::ExprStmt(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}

void ExprStmt::accept(StmtVisitor &visitor) { visitor.visit_expr_stmt(*this); }

PrintStmt::PrintStmt(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}

void PrintStmt::accept(StmtVisitor &visitor)
{
    visitor.visit_print_stmt(*this);
}

VarStmt::VarStmt(LocalVar var, std::unique_ptr<Expr> expr)
    : var(std::move(var)), expr(std::move(expr))
{
}

void VarStmt::accept(StmtVisitor &visitor) { visitor.visit_var_stmt(*this); }

BlockStmt::BlockStmt(std::vector<std::unique_ptr<Stmt>> statements)
    : statements(std::move(statements))
{
}

void BlockStmt::accept(StmtVisitor &visitor)
{
    visitor.visit_block_stmt(*this);
}

IfStmt::IfStmt(
    std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> then_branch,
    std::unique_ptr<Stmt> else_branch
)
    : condition(std::move(condition)),
      then_branch(std::move(then_branch)),
      else_branch(std::move(else_branch))
{
}

void IfStmt::accept(StmtVisitor &visitor) { visitor.visit_if_stmt(*this); }

WhileStmt::WhileStmt(
    std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body
)
    : condition(std::move(condition)),
      body(std::move(body))
{
}

void WhileStmt::accept(StmtVisitor &visitor) { visitor.visit_while_stmt(*this); }
