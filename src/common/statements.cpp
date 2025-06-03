#include "statements.hpp"

#include <memory>
#include <utility>

ExprStmt::ExprStmt(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}

void ExprStmt::accept(StmtVisitor &visitor) const
{
    visitor.visit_expr_stmt(*this);
}

PrintStmt::PrintStmt(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}

void PrintStmt::accept(StmtVisitor &visitor) const
{
    visitor.visit_print_stmt(*this);
}

VarStmt::VarStmt(LocalVar var, std::unique_ptr<Expr> expr)
    : var(std::move(var)), expr(std::move(expr))
{
}

void VarStmt::accept(StmtVisitor &visitor) const
{
    visitor.visit_var_stmt(*this);
}

BlockStmt::BlockStmt(std::vector<std::unique_ptr<Stmt>> statements)
    : statements(std::move(statements))
{
}

void BlockStmt::accept(StmtVisitor &visitor) const
{
    visitor.visit_block_stmt(*this);
}
