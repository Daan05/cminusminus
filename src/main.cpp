#include <iostream>
#include <stdexcept>

#include "common/common.hpp"
#include "lexer.hpp"

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
    lexer.lex();
}
catch (std::runtime_error const &error)
{
    std::cout << error.what() << '\n';
}
