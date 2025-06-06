#ifndef ANALYZER_HPP
#define ANALYZER_HPP

#include <vector>

#include "common/expression.hpp"
#include "common/statements.hpp"

class Analyzer
{
   public:
    Analyzer() : m_vars({}), m_scope_depth(0) {}
    ~Analyzer() = default;

    void analyze_expr(Expr &expr);
    void analyze_stmt(Stmt &stmt);

   private:
    std::vector<LocalVar> m_vars;
    int m_scope_depth;
};

#endif
