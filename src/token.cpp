#include "token.hpp"

#include <iostream>

Literal::Literal(std::string const &s) : value(s) {}

Literal::Literal(std::string &&s) : value(std::move(s)) {}

Literal::Literal(double num) : value(num) {}

Literal::Literal(Literal const &other) : value(other.value) {}

Literal::~Literal() {}

void Literal::print() const
{
    std::cout << "literal type: ";
    switch (value.index())
    {
    case 0:  // std::monostate
        std::cout << "None\n";
        break;
    case 1:  // std::string
        std::cout << "Str\n";
        std::cout << "literal value: " << get<std::string>(value) << '\n';
        break;
    case 2:  // double
        std::cout << "Num\n";
        std::cout << "literal value: " << get<double>(value) << '\n';
        break;
    }
}

// -----------

Token::Token(
    TokenType kind, std::string lexeme, Literal literal, uint32_t line,
    uint32_t column
)
    : kind(kind), lexeme(lexeme), literal(literal), line(line), column(column)
{
}

Token::~Token() {}

void Token::print() const
{
    std::cout << "kind: " << static_cast<int>(kind) << "\nlexeme: \"" << lexeme
              << "\"\n";
    literal.print();
    std::cout << "line: " << line << "\ncolumn: " << column << "\n";
}
