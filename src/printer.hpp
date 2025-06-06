#ifndef EXPR_PRINTER_HPP
#define EXPR_PRINTER_HPP

#include <string>

#include "common/expression.hpp"

class Printer
{
   public:
    Printer() = default;
    ~Printer() = default;

    std::string print_expr(Expr const &expr);
};

#endif
