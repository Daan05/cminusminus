#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <memory>
#include <vector>

#include "common/expression.hpp"
#include "common/statements.hpp"

class CodeGenerator
{
   public:
    CodeGenerator() : m_label_count(0) {};
    ~CodeGenerator() = default;

    std::string generate(std::vector<std::unique_ptr<Stmt>> const &statements);

    std::string generate_expr(Expr const &expr);
    std::string generate_stmt(Stmt const &stmt);

   private:
    size_t m_label_count;
};

#endif
