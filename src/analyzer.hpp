#ifndef ANALYZER_HPP
#define ANALYZER_HPP

#include <vector>

#include "common/expression.hpp"
#include "common/statements.hpp"

class Analyzer
{
   public:
    Analyzer() : m_had_error(0), m_vars({}), m_scope_depth(0) {}
    ~Analyzer() = default;

    void analyze(std::vector<std::unique_ptr<Stmt>> &statements);

   private:
    void analyze_expr(Expr &expr);
    void analyze_stmt(Stmt &stmt);

    bool m_had_error;
    std::vector<LocalVar> m_vars;
    int m_scope_depth;
};

#endif
