#ifndef VISITORS_HPP
#define VISITORS_HPP

#include <sstream>

#include "common/expression.hpp"
#include "common/statements.hpp"

class StmtPrinter : public StmtVisitor
{
   public:
    std::string print(Stmt &stmt);
    void visit_print_stmt(PrintStmt &stmt) override;
    void visit_expr_stmt(ExprStmt &stmt) override;
    void visit_var_stmt(VarStmt &stmt) override;
    void visit_block_stmt(BlockStmt &stmt) override;
    void visit_if_stmt(IfStmt &stmt) override;
    void visit_while_stmt(WhileStmt &stmt) override;

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
    void visit_while_stmt(WhileStmt &stmt) override;

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
    void visit_while_stmt(WhileStmt &stmt) override;

   private:
    std::ostringstream m_output;
    static int m_label_count;
};

#endif
