#ifndef TOKENTYPE_HPP
#define TOKENTYPE_HPP

#include <cstdint>
#include <string>

#include "literal.hpp"

enum class TokenType
{
    LeftParen,
    RightParen,
    LeftBrace,
    RightBrace,
    Minus,
    Plus,
    Slash,
    Star,
    Comma,
    Dot,
    Number,
    String,
    Equal,
    Bang,
    BangEqual,
    EqualEqual,
    Greater,
    GreaterEqual,
    Less,
    LessEqual,
    SemiColon,
    Colon,
    Struct,
    Fn,
    Let,
    For,
    If,
    While,
    Print,
    Return,
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
