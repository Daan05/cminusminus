#ifndef EXPR_PRINTER_HPP
#define EXPR_PRINTER_HPP

#include <string>

#include "common/expression.hpp"
#include "common/statements.hpp"

class Printer
{
   public:
    Printer() = default;
    ~Printer() = default;

    std::string print_expr(Expr const &expr);
    std::string print_stmt(Stmt const &stmt, int indent_level = 0);
};

#endif
