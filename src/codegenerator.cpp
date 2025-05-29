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
    m_output += "\t.intel_syntax noprefix\n";
    m_output += "\t.text\n";
    m_output += "\t.global main\n";
    m_output += "\t.type main, @function\n";
    m_output += "main:\n";
    m_output += "\tpush rbp # save caller's base pointer\n";
    m_output +=
        "\tmov rbp, rsp # set up a new base pointer frame for this function\n";

    // our expression arithmetic
    // walk tree
    ASTCodeGenerator astCodeGenerator;
    m_output += astCodeGenerator.generate(*m_expr);
    

    // end
    // our expression result is saved on the stack
    m_output += "\tpop rax # return value\n";
    m_output += "\tpop rbp # Restore caller's base pointer\n";
    m_output += "\tret # Return to caller (exit program)\n";
    m_output += "\t.size main, .-main\n";

    return m_output;
}
