#include "codegenerator.hpp"

#include <memory>
#include "common/expression.hpp"
#include "visitors.hpp"

CodeGenerator::CodeGenerator(std::vector<std::unique_ptr<Stmt>> statements)
    : m_statements(std::move(statements))
{
}

CodeGenerator::~CodeGenerator() {}

std::string CodeGenerator::generate()
{
    // start
    m_output << "section .data\n";
    m_output << "\tfmt db \"%d\", 10, 0     ; printf format string "
                "(with "
                "newline and null terminator)\n";
    m_output << "\nsection .text\n";
    m_output << "\tglobal main\n";
    m_output << "\textern printf\n";
    m_output << "\nmain:\n";
    m_output << "\tpush rbp ; save caller's base pointer\n";
    m_output << "\tmov rbp, rsp ; set up a new base pointer frame for this "
                "function\n\n";
    m_output << "\tsub rsp, 64 ; Reserve stack space (8 64-bit vars)\n";

    StmtCodeGenerator stmtCodeGenerator;
    for (auto const &stmt : m_statements)
    {
        m_output << stmtCodeGenerator.generate(*stmt);
    }

    m_output << "\tadd rsp, 64 ; Restore stack\n";
    m_output << "\n\tmov rax, 0 ; return value\n";
    m_output << "\tpop rbp ; Restore caller's base pointer\n";
    m_output << "\tret ; Return to caller (exit program)\n";

    return m_output.str();
}
