#include "codegenerator.hpp"

#include <sstream>

#include "common/error.hpp"

std::string CodeGenerator::generate(
    std::vector<std::unique_ptr<Stmt>> const &statements
)
{
    std::ostringstream oss;

    oss << "section .data\n";
    oss << "\tfmt db \"%d\", 10, 0     ; printf format string "
           "(with "
           "newline and null terminator)\n";
    oss << "\nsection .text\n";
    oss << "\tglobal main\n";
    oss << "\textern printf\n";
    oss << "\nmain:\n";
    oss << "\tpush rbp ; save caller's base pointer\n";
    oss << "\tmov rbp, rsp ; set up a new base pointer frame for this "
           "function\n\n";
    oss << "\tsub rsp, 2048 ; Reserve stack space (256 64-bit vars)\n";

    for (auto const &stmt : statements)
    {
        oss << generate_stmt(*stmt);
    }

    oss << "\tadd rsp, 2048 ; Restore stack\n";
    oss << "\n\tmov rax, 0 ; return value\n";
    oss << "\tpop rbp ; Restore caller's base pointer\n";
    oss << "\tret ; Return to caller (exit program)\n";

    return oss.str();
}

std::string CodeGenerator::generate_expr(const Expr &expr)
{
    std::ostringstream oss;

    switch (expr.kind)
    {
    case ExprType::Binary:
        oss << generate_expr(*expr.variant.binary.left);
        oss << generate_expr(*expr.variant.binary.right);

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
        oss << generate_expr(*expr.variant.assign.expr);
        oss << "\tpop rax\n";
        oss << "\tmov qword [rbp - " << expr.variant.assign.var.rbp_offset
            << "], rax\n";
        break;
    case ExprType::Unary:
        oss << generate_expr(*expr.variant.unary.expr);
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
        oss << generate_expr(*expr.variant.grouping.expr);
        break;
    default:
        error::unreachable();
        break;
    }

    return oss.str();
}

std::string CodeGenerator::generate_stmt(Stmt const &stmt)
{
    std::ostringstream oss;

    switch (stmt.kind)
    {
    case StmtType::Expr:
    {
        oss << generate_expr(*stmt.variant.expr.expr);
        break;
    }
    case StmtType::Print:
    {
        oss << generate_expr(*stmt.variant.print.expr);
        oss << "\tmov rdi, fmt ; 1st argument (format string)\n";
        oss << "\tpop rsi ; 2nd argument (integer to print)\n";
        oss << "\txor eax, eax ; clear rax: required before calling variadic "
               "functions like printf\n";
        oss << "\tcall printf\n";
        break;
    }
    case StmtType::Var:
    {
        oss << generate_expr(*stmt.variant.var.expr);
        oss << "\tpop rax\n";
        oss << "\tmov qword [rbp - " << stmt.variant.var.var.rbp_offset
            << "], rax\n";
        break;
    }
    case StmtType::Block:
    {
        for (auto const &stmt : stmt.variant.block.statements)
        {
            oss << generate_stmt(*stmt);
        }
        break;
    }
    case StmtType::If:
    {
        int else_label = m_label_count++;
        int end_label = m_label_count++;

        oss << generate_expr(*stmt.variant.if_.condition);

        oss << "\tpop rax\n";
        oss << "\tcmp rax, 0\n";
        oss << "\tje L" << else_label << "\n";

        oss << generate_stmt(*stmt.variant.if_.then_branch);
        oss << "\tjmp L" << end_label << "\n";

        oss << "L" << else_label << ":\n";
        if (stmt.variant.if_.else_branch != nullptr)
        {
            oss << generate_stmt(*stmt.variant.if_.else_branch);
        }

        oss << "L" << end_label << ":\n";
        break;
    }
    case StmtType::While:
    {
        int start_label = m_label_count++;
        int end_label = m_label_count++;

        oss << "L" << start_label << ":\n";

        oss << generate_expr(*stmt.variant.while_.condition);

        oss << "\tpop rax\n";
        oss << "\tcmp rax, 0\n";
        oss << "\tje L" << end_label << "\n";

        oss << generate_stmt(*stmt.variant.while_.body);

        oss << "\tjmp L" << start_label << "\n";

        oss << "L" << end_label << ":\n";
        break;
    }
    default:
    {
        error::unreachable();
        break;
    }
    }

    return oss.str();
}
