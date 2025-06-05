#ifndef VISITORS_HPP
#define VISITORS_HPP

#include <sstream>

#include "common/expression.hpp"
#include "common/statements.hpp"

class ExprPrinter : public ExprVisitor
{
   public:
    std::string print(Expr &expr);
    void visit_binary_expr(BinaryExpr &expr) override;
    void visit_literal_expr(LiteralExpr &expr) override;
    void visit_var_expr(VarExpr &expr) override;
    void visit_assign_expr(AssignExpr &expr) override;
    void visit_unary_expr(UnaryExpr &expr) override;
    void visit_grouping_expr(GroupingExpr &expr) override;

   private:
    std::ostringstream m_output;
};

class ExprAnalyzer : public ExprVisitor
{
   public:
    ExprAnalyzer(std::vector<LocalVar> &vars, int scope_depth)
        : m_vars(vars), m_scope_depth(scope_depth)
    {
    }

    void analyze(Expr &expr);
    void visit_binary_expr(BinaryExpr &expr) override;
    void visit_literal_expr(LiteralExpr &expr) override;
    void visit_var_expr(VarExpr &expr) override;
    void visit_assign_expr(AssignExpr &expr) override;
    void visit_unary_expr(UnaryExpr &expr) override;
    void visit_grouping_expr(GroupingExpr &expr) override;

   private:
    std::vector<LocalVar> &m_vars;
    int m_scope_depth;
};

class ExprCodeGenerator : public ExprVisitor
{
   public:
    std::string generate(Expr &expr);
    void visit_binary_expr(BinaryExpr &expr) override;
    void visit_literal_expr(LiteralExpr &expr) override;
    void visit_var_expr(VarExpr &expr) override;
    void visit_assign_expr(AssignExpr &expr) override;
    void visit_unary_expr(UnaryExpr &expr) override;
    void visit_grouping_expr(GroupingExpr &expr) override;

   private:
    std::ostringstream m_output;
};

class StmtPrinter : public StmtVisitor
{
   public:
    std::string print(Stmt &stmt);
    void visit_print_stmt(PrintStmt &stmt) override;
    void visit_expr_stmt(ExprStmt &stmt) override;
    void visit_var_stmt(VarStmt &stmt) override;
    void visit_block_stmt(BlockStmt &stmt) override;
    void visit_if_stmt(IfStmt &stmt) override;

   private:
    std::ostringstream m_output;
};

class StmtAnalyzer : public StmtVisitor
{
   public:
    void analyze(Stmt &stmt);
    void visit_print_stmt(PrintStmt &stmt) override;
    void visit_expr_stmt(ExprStmt &stmt) override;
    void visit_var_stmt(VarStmt &stmt) override;
    void visit_block_stmt(BlockStmt &stmt) override;
    void visit_if_stmt(IfStmt &stmt) override;

   private:
    static std::vector<LocalVar> m_vars;
    static int m_scope_depth;
};

class StmtCodeGenerator : public StmtVisitor
{
   public:
    std::string generate(Stmt &stmt);
    void visit_print_stmt(PrintStmt &stmt) override;
    void visit_expr_stmt(ExprStmt &stmt) override;
    void visit_var_stmt(VarStmt &stmt) override;
    void visit_block_stmt(BlockStmt &stmt) override;
    void visit_if_stmt(IfStmt &stmt) override;

   private:
    std::ostringstream m_output;
    static int m_label_count;
};

#endif
