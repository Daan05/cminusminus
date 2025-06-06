#ifndef EXPR_PRINTER_HPP
#define EXPR_PRINTER_HPP

#include <sstream>
#include <string>

#include "common/expression.hpp"

class ExprPrinter
{
   public:
    ExprPrinter() = default;
    ~ExprPrinter() = default;

    std::string print(Expr const &expr);
};

#endif
