#include "lexer.hpp"

#include "common/common.hpp"

#include <string>
#include <iostream>

Lexer::Lexer() {}

Lexer::~Lexer() {}

void Lexer::lex(std::string const &path) {
    std::string file = common::read_file(path);
    std::cout << file << '\n';
}
