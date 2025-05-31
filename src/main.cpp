#include <iostream>
#include <memory>
#include <stdexcept>

#include "codegenerator.hpp"
#include "common/common.hpp"
#include "common/expression.hpp"
#include "common/token.hpp"
#include "lexer.hpp"
#include "parser.hpp"

#define DEBUG_TOKENS 0
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
    std::unique_ptr<Expr> expressions = parser.parse();

#if DEBUG_AST
    ASTPrinter printer;
    std::cout << printer.print(*expressions);
#endif

    CodeGenerator generator(std::move(expressions));
    std::string asm_code = generator.generate();

    common::write_file("test.asm", asm_code);
}
catch (std::runtime_error const &error)
{
    std::cout << error.what() << '\n';
}
