#include "codegenerator.hpp"
#include <memory>

CodeGenerator::CodeGenerator(std::unique_ptr<Expr> expr)
    : m_expr(std::move(expr))
{
}

CodeGenerator::~CodeGenerator() {}

std::string CodeGenerator::generate()
{
    // start
    std::string m_output = "";
    m_output += "section .data\n";
    m_output +=
        "\tfmt db \"Result: %d\", 10, 0     ; printf format string (with "
        "newline and null terminator)\n";
    m_output += "\nsection .text\n";
    m_output += "\tglobal main\n";
    m_output += "\textern printf\n";
    m_output += "\nmain:\n";
    m_output += "\tpush rbp ; save caller's base pointer\n";
    m_output +=
        "\tmov rbp, rsp ; set up a new base pointer frame for this function\n";

    // our expression arithmetic
    // walk tree
    ASTCodeGenerator astCodeGenerator;
    m_output += astCodeGenerator.generate(*m_expr);

    // end
    // our expression result is saved on the stack
    m_output += "\tmov rdi, fmt ; 1st argument (format string)\n";
    m_output += "\tpop rsi ; 2nd argument (integer to print)\n";
    m_output += "\txor eax, eax ; Clear RAX: required before calling variadic functions like printf\n";
    m_output += "\tcall printf\n";
    m_output += "\tmov rax, 0 ; return value\n";
    m_output += "\tpop rbp ; Restore caller's base pointer\n";
    m_output += "\tret ; Return to caller (exit program)\n";

    return m_output;
}
