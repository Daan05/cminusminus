#include "generator.hpp"

#include "common/error.hpp"

std::string ExprCodeGenerator::generate(const Expr &expr)
{
    std::ostringstream oss;

    switch (expr.kind)
    {
    case ExprType::Binary:
        oss << generate(*expr.variant.binary.left);
        oss << generate(*expr.variant.binary.right);

        oss << "\tpop rax\n";
        oss << "\tpop rcx\n";
        switch (expr.variant.binary.op.kind)
        {
        case TokenType::Plus:
            oss << "\tadd rax, rcx\n";
            oss << "\tpush rax\n";
            break;
        case TokenType::Minus:
            oss << "\tsub rcx, rax\n";
            oss << "\tpush rcx\n";
            break;
        case TokenType::Star:
            oss << "\timul rax, rcx\n";
            oss << "\tpush rax\n";
            break;
        case TokenType::Slash:
            error::todo(
                "ExprCodeGenerator::visit_binary_expr(): add support for "
                "division"
            );
            break;
        case TokenType::EqualEqual:
            oss << "\tcmp rcx, rax\n";
            oss << "\tsete al\n";
            oss << "\tmovzx rax, al\n";
            oss << "\tpush rax\n";
            break;
        case TokenType::BangEqual:
            oss << "\tcmp rcx, rax\n";
            oss << "\tsetne al\n";
            oss << "\tmovzx rax, al\n";
            oss << "\tpush rax\n";
            break;
        case TokenType::Less:
            oss << "\tcmp rcx, rax\n";
            oss << "\tsetl al\n";
            oss << "\tmovzx rax, al\n";
            oss << "\tpush rax\n";
            break;
        case TokenType::LessEqual:
            oss << "\tcmp rcx, rax\n";
            oss << "\tsetle al\n";
            oss << "\tmovzx rax, al\n";
            oss << "\tpush rax\n";
            break;
        case TokenType::Greater:
            oss << "\tcmp rcx, rax\n";
            oss << "\tsetg al\n";
            oss << "\tmovzx rax, al\n";
            oss << "\tpush rax\n";
            break;
        case TokenType::GreaterEqual:
            oss << "\tcmp rcx, rax\n";
            oss << "\tsetge al\n";
            oss << "\tmovzx rax, al\n";
            oss << "\tpush rax\n";
            break;
        case TokenType::BoolAnd:
            // TODO: short circuit evaluation
            oss << "\tcmp rcx, 0\n";
            oss << "\tsete cl\n";
            oss << "\tcmp rax, 0\n";
            oss << "\tsete al\n";
            oss << "\tor al, cl\n";
            oss << "\txor al, 1\n";
            oss << "\tmovzx rax, al\n";
            oss << "\tpush rax\n";
            break;
        case ::TokenType::BoolOr:
            // TODO: short circuit evaluation
            oss << "\tcmp rcx, 0\n";
            oss << "\tsetne cl\n";
            oss << "\tcmp rax, 0\n";
            oss << "\tsetne al\n";
            oss << "\tor al, cl\n";
            oss << "\tmovzx rax, al\n";
            oss << "\tpush rax\n";
            break;
        default:
            error::unreachable();
        }
        break;
    case ExprType::Literal:
        oss << "\tpush " << expr.variant.literal.token.literal.to_string()
            << '\n';
        break;
    case ExprType::Var:
        oss << "\tmov rax, qword [rbp - " << expr.variant.var.var.rbp_offset
            << "]\n";
        oss << "\tpush rax\n";
        break;
    case ExprType::Assign:
        oss << generate(*expr.variant.assign.expr);
        oss << "\tpop rax\n";
        oss << "\tmov qword [rbp - " << expr.variant.assign.var.rbp_offset
            << "], rax\n";
        break;
    case ExprType::Unary:
        oss << generate(*expr.variant.unary.expr);
        switch (expr.variant.unary.op.kind)
        {
        case TokenType::Minus:
            oss << "\tpop rax\n";
            oss << "\tneg rax\n";
            oss << "\tpush rax\n";
            break;
        case TokenType::Bang:
            error::todo("ExprCodeGenerator::visit_unary_expr: add bang");
            break;
        default:
            error::unreachable();
            break;
        }
        break;
    case ExprType::Grouping:
        oss << generate(*expr.variant.grouping.expr);
        break;
    default:
        oss << "(unknown)";
        break;
    }

    return oss.str();
}
