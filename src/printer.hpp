#ifndef EXPR_PRINTER_HPP
#define EXPR_PRINTER_HPP

#include <memory>
#include <string>
#include <vector>

#include "common/expression.hpp"
#include "common/statements.hpp"

class Printer
{
   public:
    Printer() = default;
    ~Printer() = default;

    void print(std::vector<std::unique_ptr<Stmt>> const &statements);

    std::string print_expr(Expr const &expr);
    std::string print_stmt(Stmt const &stmt, int indent_level = 0);
};

#endif
