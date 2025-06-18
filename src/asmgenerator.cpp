#include "asmgenerator.hpp"

#include <iostream>
#include <sstream>
#include <set>

#include "common/irinstructions.hpp"

std::string generate_assembly(
    std::pair<
        std::vector<std::unique_ptr<IRInstr>>,
        std::unordered_map<std::string, IRFunction>> &ir
)
{
    std::ostringstream oss;

    oss << "section .data\n";
    oss << "fmt: db \"%ld\", 10, 0\n";
    std::set<std::string> vars;
    for (auto &instr : ir.first)
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
        case IRType::Call:
            vars.insert(instr->variant.call.dst);
            for (auto const &arg : instr->variant.call.args)
            {
                vars.insert(arg);
            }
            break;
        case IRType::Return:
            break;
        }
    }
    for (auto &func : ir.second)
    {
        for (auto &instr : func.second.body)
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
            case IRType::Call:
                vars.insert(instr->variant.call.dst);
                for (auto const &arg : instr->variant.call.args)
                {
                    vars.insert(arg);
                }
                break;
            case IRType::Return:
                break;
            }
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

    for (auto &func : ir.second)
    {
        for (auto &instr : func.second.body)
        {
            switch (instr->kind)
            {
            case IRType::Assign:
            {
                auto &a = instr->variant.assign;
                if (isdigit(a.src[0]) || (a.src[0] == '-' && isdigit(a.src[1])))
                {
                    oss << "\tmov rax, " << a.src << "\n";
                }
                else
                {
                    oss << "\tmov rax, [" << a.src << "]\n";
                }
                oss << "\tmov [" << a.dst << "], rax\n";
                break;
            }
            case IRType::BinaryOp:
            {
                auto &a = instr->variant.binaryOp;

                oss << "\tmov rax, [" << a.left << "]\n";
                if (a.op == "==" || a.op == "!=" || a.op == "<" ||
                    a.op == ">" || a.op == "<=" || a.op == ">=")
                {
                    oss << "\tcmp rax, [" << a.right << "]\n";
                    if (a.op == "==")
                        oss << "\tsete al\n";
                    else if (a.op == "!=")
                        oss << "\tsetne al\n";
                    else if (a.op == "<")
                        oss << "\tsetl al\n";
                    else if (a.op == "<=")
                        oss << "\tsetle al\n";
                    else if (a.op == ">")
                        oss << "\tsetg al\n";
                    else if (a.op == ">=")
                        oss << "\tsetge al\n";
                    oss << "\tmovzx rax, al\n";
                }
                else if (a.op == "+")
                {
                    oss << "\tmov rbx, [" << a.right << "]\n";
                    oss << "\tadd rax, rbx\n";
                }
                else if (a.op == "-")
                {
                    oss << "\tmov rbx, [" << a.right << "]\n";
                    oss << "\tsub rax, rbx\n";
                }
                else if (a.op == "*")
                {
                    oss << "\tmov rbx, [" << a.right << "]\n";
                    oss << "\timul rax, rbx\n";
                }
                else
                {
                    oss << "\t; unsupported binary op: " << a.op << "\n";
                }

                oss << "\tmov [" << a.dst << "], rax\n";
                break;
            }
            case IRType::UnaryOp:
            {
                auto &a = instr->variant.unaryOp;
                oss << "\tmov rax, [" << a.value << "]\n";
                if (a.op == "-")
                {
                    oss << "\tneg rax\n";
                }
                oss << "\tmov [" << a.dst << "], rax\n";
                break;
            }
            case IRType::Goto:
            {
                auto &a = instr->variant.goto_;
                oss << "\tjmp " << a.label << "\n";
                break;
            }
            case IRType::IfFalseGoto:
            {
                auto &a = instr->variant.ifFalseGoto;
                oss << "\tcmp qword [" << a.condition << "], 0\n";
                oss << "\tje " << a.label << "\n";
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
                oss << "\tmov rdi, fmt\n";
                oss << "\tmov rsi, [" << a.value << "]\n";
                oss << "\txor rax, rax\n";
                oss << "\tcall printf\n";
                break;
            }
            case IRType::Call:
            {
            }
            case IRType::Return:
                oss << "\tret\n";
                break;
            }
        }
        oss << '\n';
    }

    oss << "_start:\n";

    for (auto &instr : ir.first)
    {
        switch (instr->kind)
        {
        case IRType::Assign:
        {
            auto &a = instr->variant.assign;
            if (isdigit(a.src[0]) || (a.src[0] == '-' && isdigit(a.src[1])))
            {
                oss << "\tmov rax, " << a.src << "\n";
            }
            else
            {
                oss << "\tmov rax, [" << a.src << "]\n";
            }
            oss << "\tmov [" << a.dst << "], rax\n";
            break;
        }
        case IRType::BinaryOp:
        {
            auto &a = instr->variant.binaryOp;

            oss << "\tmov rax, [" << a.left << "]\n";
            if (a.op == "==" || a.op == "!=" || a.op == "<" || a.op == ">" ||
                a.op == "<=" || a.op == ">=")
            {
                oss << "\tcmp rax, [" << a.right << "]\n";
                if (a.op == "==")
                    oss << "\tsete al\n";
                else if (a.op == "!=")
                    oss << "\tsetne al\n";
                else if (a.op == "<")
                    oss << "\tsetl al\n";
                else if (a.op == "<=")
                    oss << "\tsetle al\n";
                else if (a.op == ">")
                    oss << "\tsetg al\n";
                else if (a.op == ">=")
                    oss << "\tsetge al\n";
                oss << "\tmovzx rax, al\n";
            }
            else if (a.op == "+")
            {
                oss << "\tmov rbx, [" << a.right << "]\n";
                oss << "\tadd rax, rbx\n";
            }
            else if (a.op == "-")
            {
                oss << "\tmov rbx, [" << a.right << "]\n";
                oss << "\tsub rax, rbx\n";
            }
            else if (a.op == "*")
            {
                oss << "\tmov rbx, [" << a.right << "]\n";
                oss << "\timul rax, rbx\n";
            }
            else
            {
                oss << "\t; unsupported binary op: " << a.op << "\n";
            }

            oss << "\tmov [" << a.dst << "], rax\n";
            break;
        }
        case IRType::UnaryOp:
        {
            auto &a = instr->variant.unaryOp;
            oss << "\tmov rax, [" << a.value << "]\n";
            if (a.op == "-")
            {
                oss << "\tneg rax\n";
            }
            oss << "t\tmov [" << a.dst << "], rax\n";
            break;
        }
        case IRType::Goto:
        {
            auto &a = instr->variant.goto_;
            oss << "\tjmp " << a.label << "\n";
            break;
        }
        case IRType::IfFalseGoto:
        {
            auto &a = instr->variant.ifFalseGoto;
            oss << "\tcmp qword [" << a.condition << "], 0\n";
            oss << "\tje " << a.label << "\n";
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
            oss << "\tmov rdi, fmt\n";
            oss << "\tmov rsi, [" << a.value << "]\n";
            oss << "\txor rax, rax\n";
            oss << "\tcall printf\n";
            break;
        }
        case IRType::Call:
        {
            auto &a = instr->variant.call;
            auto &f = ir.second[a.funcName];

            for (size_t i = 0; i < a.args.size(); ++i)
            {
                oss << "\tmov rax, [" << a.args[i] << "]\n";
                oss << "\tmov [" << f.params[i] << "], rax\n";
            }

            oss << "\tcall " << "func_" << a.funcName << "\n";

            if (!a.dst.empty())
            {
                oss << "\tmov rax, [t0]\n";
                oss << "\tmov [" << a.dst << "], rax\n";
            }
            break;
        }
        case IRType::Return:
            oss << "\tret\n";
            break;
        }
    }

    oss << "\tmov rax, 60\n";
    oss << "\txor rdi, rdi\n";
    oss << "\tsyscall\n\n";
    oss << "\tsection .bss\n";
    oss << "\tbuffer resb 20\n";

    return oss.str();
}
