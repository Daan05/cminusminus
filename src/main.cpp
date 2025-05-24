#include <cstddef>
#include <cstdlib>
#include <iostream>

#include "common/common.hpp"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "c--: " << "\x1B[31m" << "fatal error: " << "\033[0m"
                  << "no input files\n";
        return EXIT_FAILURE;
    }

    for (size_t ix = 1; ix != static_cast<size_t>(argc); ++ix)
    {
        std::cout << argv[ix] << '\n';
    }

    std::string file = common::read_file(argv[1]);
    std::cout << file << '\n';

    return EXIT_SUCCESS;
}
