#include "statements.hpp"

#include <memory>
#include <utility>

ExprStmt::ExprStmt(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}

void ExprStmt::accept(StmtVisitor &visitor) const { visitor.visit_expr_stmt(*this); }

PrintStmt::PrintStmt(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}

void PrintStmt::accept(StmtVisitor &visitor) const { visitor.visit_print_stmt(*this); }

VarStmt::VarStmt(Var var, std::unique_ptr<Expr> expr) : var(std::move(var)), expr(std::move(expr)) {}

void VarStmt::accept(StmtVisitor &visitor) const { visitor.visit_var_stmt(*this); }
