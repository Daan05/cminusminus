#ifndef CODEGENERATOR_HPP
#define CODEGENERATOR_HPP

#include <memory>

#include "common/expression.hpp"

class CodeGenerator
{
   public:
    CodeGenerator(std::unique_ptr<Expr> expr);
    ~CodeGenerator();

    std::string generate();

   private:
    std::unique_ptr<Expr> m_expr;
    std::string m_output;

   private:
    // member function
};

#endif
