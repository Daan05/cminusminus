#include <iostream>

#include "common/error.hpp"
#include "codegenerator.hpp"
#include "common/common.hpp"
#include "common/token.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "visitors.hpp"

#define DEBUG_TOKENS 0
#define DEBUG_AST 0

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
    StmtPrinter stmtPrinter;
    for (auto const &stmt : statements.first)
    {
        std::cout << stmtPrinter.print(*stmt);
    }
#endif

    CodeGenerator generator(std::move(statements));
    std::string asm_code = generator.generate();

    common::write_file("test.asm", asm_code);
}
catch (error::Fatal const &error)
{
}
catch (...)
{
    std::cout << "Unknown exception occured\n";
}
