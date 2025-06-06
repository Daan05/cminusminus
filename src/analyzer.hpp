#ifndef ANALYZER_HPP
#define ANALYZER_HPP

#include <vector>

#include "common/expression.hpp"

class ExprAnalyzer
{
   public:
    ExprAnalyzer(std::vector<LocalVar> &vars, int scope_depth)
        : m_vars(vars), m_scope_depth(scope_depth)
    {
    }
    ~ExprAnalyzer() = default;

    void analyze(Expr &expr);

   private:
    std::vector<LocalVar> &m_vars;
    int m_scope_depth;
};

#endif
