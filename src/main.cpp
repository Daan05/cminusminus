#include <iostream>
#include <memory>
#include <stdexcept>

#include "codegenerator.hpp"
#include "common/common.hpp"
#include "common/token.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "visitors.hpp"

#define DEBUG_TOKENS 1
#define DEBUG_AST 0

int main(int argc, char **argv)
try
{
    if (argc < 2)
    {
        throw std::runtime_error(
            "c--: \x1B[31mfatal error: \033[0mno input files"
        );
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
    std::vector<std::unique_ptr<Stmt>> statements = parser.parse();

#if DEBUG_AST
    StmtPrinter stmtPrinter;
    for (auto const &stmt : statements)
    {
        std::cout << stmtPrinter.print(*stmt);
    }
#endif

    CodeGenerator generator(std::move(statements));
    std::string asm_code = generator.generate();

    common::write_file("test.asm", asm_code);
}
catch (std::runtime_error const &error)
{
    std::cout << error.what() << '\n';
}
