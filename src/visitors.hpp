#ifndef VISITORS_HPP
#define VISITORS_HPP

#include <sstream>

#include "common/expression.hpp"
#include "common/statements.hpp"

class ExprPrinter : public ExprVisitor
{
   public:
    std::string print(Expr const &expr);
    void visit_binary_expr(BinaryExpr const &expr) override;
    void visit_literal_expr(LiteralExpr const &expr) override;
    void visit_var_expr(VarExpr const &expr) override;
    void visit_assign_expr(AssignExpr const &expr) override;
    void visit_unary_expr(UnaryExpr const &expr) override;
    void visit_grouping_expr(GroupingExpr const &expr) override;

   private:
    std::ostringstream m_output;
};

class ExprCodeGenerator : public ExprVisitor
{
   public:
    std::string generate(Expr const &expr);
    void visit_binary_expr(BinaryExpr const &expr) override;
    void visit_literal_expr(LiteralExpr const &expr) override;
    void visit_var_expr(VarExpr const &expr) override;
    void visit_assign_expr(AssignExpr const &expr) override;
    void visit_unary_expr(UnaryExpr const &expr) override;
    void visit_grouping_expr(GroupingExpr const &expr) override;

   private:
    std::ostringstream m_output;
};

class StmtPrinter : public StmtVisitor
{
public:
    std::string print(Stmt const &stmt);
    void visit_print_stmt(PrintStmt const &stmt) override;
    void visit_expr_stmt(ExprStmt const &stmt) override;
    void visit_var_stmt(VarStmt const &stmt) override;

private:
    std::ostringstream m_output;
};

class StmtCodeGenerator : public StmtVisitor
{
public:
    std::string generate(Stmt const &stmt);
    void visit_print_stmt(PrintStmt const &stmt) override;
    void visit_expr_stmt(ExprStmt const &stmt) override;
    void visit_var_stmt(VarStmt const &stmt) override;

private:
    std::ostringstream m_output;
};

#endif
