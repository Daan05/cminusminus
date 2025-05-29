#include <iostream>
#include <stdexcept>

#include "common/common.hpp"
#include "common/expression.hpp"
#include "lexer.hpp"
#include "parser.hpp"

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
    for (Token const &token : tokens)
    {
        token.print();
        std::cout << '\n';
    }

    Parser parser(tokens);
    Expr expressions = parser.parse();


}
catch (std::runtime_error const &error)
{
    std::cout << error.what() << '\n';
}
