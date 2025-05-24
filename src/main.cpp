#include <iostream>
#include <stdexcept>

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

    Lexer lexer;
    for (size_t ix = 1; ix != static_cast<size_t>(argc); ++ix)
    {
        if (argv[ix][0] != '-')
            lexer.lex(argv[ix]);
    }
}
catch (std::runtime_error const &error)
{
    std::cout << error.what() << '\n';
}
