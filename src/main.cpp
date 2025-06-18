#include <iostream>

#include "common/error.hpp"
#include "asmgenerator.hpp"
#include "irgenerator.hpp"
#include "common/common.hpp"
#include "common/token.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "analyzer.hpp"
#include "printer.hpp"

#define DEBUG_TOKENS 0
#define DEBUG_AST 1

int main(int argc, char **argv)
try
{
    // TODO: properly handle input
    if (argc < 2)
    {
        error::fatal("No input files");
    }

    std::string source = common::read_file(argv[1]);

    Lexer lexer(source);
    std::vector<Token> tokens = lexer.lex();

#if DEBUG_TOKENS
    std::cout << '\n';
    for (Token const &token : tokens)
    {
        token.print();
    }
    std::cout << '\n';
#endif

    Parser parser(tokens);
    auto statements = parser.parse();

#if DEBUG_AST
    Printer printer;
    printer.print(statements);
    std::cout << "\n\n";
#endif

    Analyzer analyzer;
    analyzer.analyze(statements);

    IRGenerator generator;
    auto ir = generator.generate(statements);
    for (auto const &func : ir.second)
    {
        for (auto const &instr : func.second.body)
        {
            std ::cout << instr->to_string() << '\n';
        }
         std::cout << '\n';
    }
    for (auto const &instr : ir.first)
    {
        std ::cout << instr->to_string() << '\n';
    }
    std::cout << "\n\n";

    std::string asm_code = generate_assembly(ir);

    common::write_file("test.asm", asm_code);
}
catch (error::Fatal const &error)
{
}
catch (...)
{
    std::cout << "Unknown exception occured\n";
}
