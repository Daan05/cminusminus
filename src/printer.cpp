#include "printer.hpp"

#include <iostream>
#include <sstream>

#include "common/error.hpp"
#include "common/statements.hpp"
#include "common/expression.hpp"

void Printer::print(std::vector<std::unique_ptr<Stmt>> const &statements)
{
    for (auto const &stmt : statements)
    {
        std::cout << print_stmt(*stmt);
    }
}

std::string indent(int level) { return std::string(level * 4, ' '); }

std::string Printer::print_expr(Expr const &expr)
{
    std::ostringstream oss;

    switch (expr.kind)
    {
    case ExprType::Binary:
        oss << "(" << expr.variant.binary.op.lexeme << " "
            << print_expr(*expr.variant.binary.left) << " "
            << print_expr(*expr.variant.binary.right) << ")";
        break;
    case ExprType::Literal:
        oss << expr.variant.literal.token.lexeme;
        break;
    case ExprType::Var:
        oss << expr.variant.var.var.token.lexeme;
        break;
    case ExprType::Assign:
        oss << "(= " << expr.variant.assign.var.token.lexeme << " "
            << print_expr(*expr.variant.assign.expr) << ")";
        break;
    case ExprType::Unary:
        oss << "(" << expr.variant.unary.op.lexeme << " "
            << print_expr(*expr.variant.unary.expr) << ")";
        break;
    case ExprType::Grouping:
        oss << "(group " << print_expr(*expr.variant.grouping.expr) << ")";
        break;
    case ExprType::Call:
        oss << expr.variant.call.callee->variant.var.var.token.lexeme << "()";
        break;
    default:
        error::unreachable();
        break;
    }

    return oss.str();
}

std::string Printer::print_stmt(Stmt const &stmt, int indent_level)
{
    std::ostringstream oss;
    std::string pad = indent(indent_level);

    switch (stmt.kind)
    {
    case StmtType::Expr:
        oss << pad << "EXPR: ";
        oss << print_expr(*stmt.variant.expr.expr) << '\n';
        break;
    case StmtType::Print:
        oss << pad << "PRINT: ";
        oss << print_expr(*stmt.variant.print.expr) << '\n';
        break;
    case StmtType::Var:
        oss << pad << "VAR DECL: ";
        oss << "let " << stmt.variant.var.var.token.lexeme << " = ";
        oss << print_expr(*stmt.variant.var.expr) << '\n';
        break;
    case StmtType::Block:
        oss << pad << "BLOCK {\n";
        for (auto &s : stmt.variant.block.statements)
        {
            oss << print_stmt(*s, indent_level + 1);
        }
        oss << pad << "}\n";
        break;
    case StmtType::If:
        oss << pad << "IF ";
        oss << print_expr(*stmt.variant.if_.condition) << '\n';
        oss << print_stmt(*stmt.variant.if_.then_branch, indent_level);
        if (stmt.variant.if_.else_branch != nullptr)
        {
            oss << pad << "ELSE\n";
            oss << print_stmt(*stmt.variant.if_.else_branch, indent_level);
        }
        break;
    case StmtType::While:
        oss << pad << "WHILE ";
        oss << print_expr(*stmt.variant.while_.condition) << '\n';
        oss << print_stmt(*stmt.variant.while_.body, indent_level);
        break;
    case StmtType::Func:
        oss << pad << "FUNC DECL: " << stmt.variant.func.name.lexeme << "(";
        for (auto const &param : stmt.variant.func.params)
        {
            oss << param.lexeme << " ";
        }
        oss << ")\n";
        oss << print_stmt(*stmt.variant.func.body);
        break;
    default:
        error::unreachable();
        break;
    }

    return oss.str();
}
