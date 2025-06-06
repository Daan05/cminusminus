#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <sstream>

#include "common/expression.hpp"

class ExprCodeGenerator
{
   public:
    ExprCodeGenerator() = default;
    ~ExprCodeGenerator() = default;

    std::string generate(Expr const &expr);
};

#endif
