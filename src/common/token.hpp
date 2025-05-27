#ifndef TOKENTYPE_HPP
#define TOKENTYPE_HPP

#include <cstdint>
#include <string>

#include "literal.hpp"

enum class TokenType
{
    Minus,
    Plus,
    Slash,
    Star,
    Number,
    Eof,
};

struct Token
{
    Token(
        TokenType kind, std::string lexeme, Literal literal, uint32_t line,
        uint32_t column
    );
    ~Token();

    TokenType kind;
    std::string lexeme;
    Literal literal;
    uint32_t line;
    uint32_t column;

    void print() const;
};

#endif
