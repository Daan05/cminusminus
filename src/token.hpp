#ifndef TOKENTYPE_HPP
#define TOKENTYPE_HPP

#include <cstdint>
#include <string>
#include <variant>

using LiteralValue = std::variant<std::monostate, std::string, double>;

struct Literal
{
    Literal() = default;
    Literal(std::string const &s);
    Literal(std::string &&s);
    Literal(double);
    Literal(Literal const &Literal);
    ~Literal();

    LiteralValue value;

    void print() const;
};

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
