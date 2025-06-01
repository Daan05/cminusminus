#include "token.hpp"

#include <iostream>

std::string TT_to_string(TokenType kind)
{
    switch (kind)
    {
    case TokenType::LeftParen:
        return "LeftParen";
    case TokenType::RightParen:
        return "RightParen";
    case TokenType::LeftBrace:
        return "LeftBrace";
    case TokenType::RightBrace:
        return "RightBrace";
    case TokenType::Minus:
        return "Minus";
    case TokenType::Plus:
        return "Plus";
    case TokenType::Slash:
        return "Slash";
    case TokenType::Star:
        return "Star";
    case TokenType::Comma:
        return "Comma";
    case TokenType::Identifier:
        return "Identifier";
    case TokenType::Dot:
        return "Dot";
    case TokenType::Number:
        return "Number";
    case TokenType::String:
        return "String";
    case TokenType::Equal:
        return "Equal";
    case TokenType::Bang:
        return "Bang";
    case TokenType::BangEqual:
        return "BangEqual";
    case TokenType::EqualEqual:
        return "EqualEqual";
    case TokenType::Greater:
        return "Greater";
    case TokenType::GreaterEqual:
        return "GreaterEqual";
    case TokenType::Less:
        return "Less";
    case TokenType::LessEqual:
        return "LessEqual";
    case TokenType::SemiColon:
        return "SemiColon";
    case TokenType::Colon:
        return "Colon";
    case TokenType::Struct:
        return "Struct";
    case TokenType::Fn:
        return "Fn";
    case TokenType::Let:
        return "Let";
    case TokenType::For:
        return "For";
    case TokenType::If:
        return "If";
    case TokenType::Else:
        return "Else";
    case TokenType::While:
        return "While";
    case TokenType::Print:
        return "Print";
    case TokenType::Return:
        return "Return";
    case TokenType::True:
        return "True";
    case TokenType::False:
        return "False";
    case TokenType::Eof:
        return "Eof";
    default:
        // unreachable
        return "";
    }
}

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
    std::cout << "{ kind: " << TT_to_string(kind) << ", lexeme: \"" << lexeme
              << "\", ";
    literal.print();
    std::cout << ", line: " << line << ", column: " << column << " }\n";
}
