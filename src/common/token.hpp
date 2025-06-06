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
    Identifier,
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
    BoolAnd,
    BoolOr,
    LogicAnd,
    LogicOr,
    LogicXor,
    LogicNeg,
    SemiColon,
    Colon,
    Struct,
    Fn,
    Let,
    For,
    If,
    Else,
    While,
    Print,
    Return,
    True,
    False,
    Eof,
};

constexpr std::string TT_to_string(TokenType kind);

struct Token
{
    Token(
        TokenType kind, std::string lexeme, Literal literal, uint32_t line,
        uint32_t column
    );
    Token() = default;
    ~Token();

    TokenType kind;
    std::string lexeme;
    Literal literal;
    uint32_t line;
    uint32_t column;

    void print() const;
};

#endif
