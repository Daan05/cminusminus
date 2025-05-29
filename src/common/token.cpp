#include "token.hpp"

#include <iostream>

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
    std::cout << "kind: " << static_cast<int>(kind) << " lexeme: \"" << lexeme
              << "\" ";
    literal.print();
    std::cout << " line: " << line << " column: " << column << "\n";
}
