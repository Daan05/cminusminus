#include "asmgenerator.hpp"

#include <sstream>
#include <set>
#include "common/error.hpp"

std::string generate_assembly(const std::vector<std::unique_ptr<IRInstr>> &ir)
{
    std::ostringstream oss;

    oss << "section .data\n";
    oss << "fmt: db \"%ld\", 10, 0\n";
    std::set<std::string> vars;
    for (auto &instr : ir)
    {
        switch (instr->kind)
        {
        case IRType::Assign:
            vars.insert(instr->variant.assign.dst);
            vars.insert(instr->variant.assign.src);
            break;
        case IRType::BinaryOp:
            vars.insert(instr->variant.binaryOp.dst);
            vars.insert(instr->variant.binaryOp.left);
            vars.insert(instr->variant.binaryOp.right);
            break;
        case IRType::UnaryOp:
            vars.insert(instr->variant.unaryOp.dst);
            vars.insert(instr->variant.unaryOp.value);
            break;
        case IRType::Goto:
            break;
        case IRType::IfFalseGoto:
            vars.insert(instr->variant.ifFalseGoto.condition);
            break;
        case IRType::Label:
            break;
        case IRType::Print:
            vars.insert(instr->variant.print.value);
            break;
        default:
            error::unreachable();
        }
    }

    for (auto const &var : vars)
    {
        if ((!var.empty() && var[0] == 't') || isalpha(var[0]))
        {
            oss << var << ": dq 0\n";
        }
    }

    oss << "newline: db 10\n\n";
    oss << "section .text\n";
    oss << "extern printf\n";
    oss << "global _start\n\n";
    oss << "_start:\n";

    for (auto &instr : ir)
    {
        switch (instr->kind)
        {
        case IRType::Assign:
        {
            auto &a = instr->variant.assign;
            if (isdigit(a.src[0]) || (a.src[0] == '-' && isdigit(a.src[1])))
            {
                oss << "    mov rax, " << a.src << "\n";
            }
            else
            {
                oss << "    mov rax, [" << a.src << "]\n";
            }
            oss << "    mov [" << a.dst << "], rax\n";
            break;
        }
        case IRType::BinaryOp:
        {
            auto &a = instr->variant.binaryOp;

            oss << "    mov rax, [" << a.left << "]\n";
            if (a.op == "==" || a.op == "!=" || a.op == "<" || a.op == ">" ||
                a.op == "<=" || a.op == ">=")
            {
                oss << "    cmp rax, [" << a.right << "]\n";
                if (a.op == "==")
                    oss << "    sete al\n";
                else if (a.op == "!=")
                    oss << "    setne al\n";
                else if (a.op == "<")
                    oss << "    setl al\n";
                else if (a.op == "<=")
                    oss << "    setle al\n";
                else if (a.op == ">")
                    oss << "    setg al\n";
                else if (a.op == ">=")
                    oss << "    setge al\n";
                oss << "    movzx rax, al\n";
            }
            else if (a.op == "+")
            {
                oss << "    mov rbx, [" << a.right << "]\n";
                oss << "    add rax, rbx\n";
            }
            else if (a.op == "-")
            {
                oss << "    mov rbx, [" << a.right << "]\n";
                oss << "    sub rax, rbx\n";
            }
            else if (a.op == "*")
            {
                oss << "    mov rbx, [" << a.right << "]\n";
                oss << "    imul rax, rbx\n";
            }
            else
            {
                oss << "    ; unsupported binary op: " << a.op << "\n";
            }

            oss << "    mov [" << a.dst << "], rax\n";
            break;
        }
        case IRType::UnaryOp:
        {
            auto &a = instr->variant.unaryOp;
            oss << "    mov rax, [" << a.value << "]\n";
            if (a.op == "-")
            {
                oss << "    neg rax\n";
            }
            oss << "    mov [" << a.dst << "], rax\n";
            break;
        }
        case IRType::Goto:
        {
            auto &a = instr->variant.goto_;
            oss << "    jmp " << a.label << "\n";
            break;
        }
        case IRType::IfFalseGoto:
        {
            auto &a = instr->variant.ifFalseGoto;
            oss << "    cmp qword [" << a.condition << "], 0\n";
            oss << "    je " << a.label << "\n";
            break;
        }
        case IRType::Label:
        {
            auto &a = instr->variant.label;
            oss << a.name << ":\n";
            break;
        }
        case IRType::Print:
        {
            auto &a = instr->variant.print;
            oss << "    mov rdi, fmt\n";
            oss << "    mov rsi, [" << a.value << "]\n";
            oss << "    xor rax, rax\n";
            oss << "    call printf\n";
            break;
        }
        default:
        {
            error::unreachable();
        }
        }
    }

    oss << R"(    ; exit(0)
    mov rax, 60
    xor rdi, rdi
    syscall

section .bss
buffer resb 20
)";

    return oss.str();
}
