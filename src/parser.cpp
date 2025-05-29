#include "parser.hpp"

#include <format>
#include <memory>
#include <stdexcept>
#include <string>

#include "common/expression.hpp"
#include "common/token.hpp"

Parser::Parser(std::vector<Token> tokens)
    : tokens(std::move(tokens)), current(0)
{
}

Parser::~Parser() {}

std::unique_ptr<Expr> Parser::parse() { return parse_expr(); }

std::unique_ptr<Expr> Parser::parse_expr() { return parse_equality(); }

std::unique_ptr<Expr> Parser::parse_equality()
{
    auto expr = parse_comparison();

    while (match({TokenType::BangEqual, TokenType::EqualEqual}))
    {
        Token op = previous();
        auto right = parse_comparison();
        expr = std::make_unique<BinaryExpr>(
            std::move(expr), op, std::move(right), op.line
        );
    }

    return expr;
}

std::unique_ptr<Expr> Parser::parse_comparison()
{
    auto expr = parse_term();

    while (match(
        {TokenType::Greater, TokenType::GreaterEqual, TokenType::Less,
         TokenType::LessEqual}
    ))
    {
        Token op = previous();
        auto right = parse_term();
        expr = std::make_unique<BinaryExpr>(
            std::move(expr), op, std::move(right), op.line
        );
    }

    return expr;
}

std::unique_ptr<Expr> Parser::parse_term()
{
    auto expr = parse_factor();

    while (match({TokenType::Plus, TokenType::Minus}))
    {
        Token op = previous();
        auto right = parse_factor();
        expr = std::make_unique<BinaryExpr>(
            std::move(expr), op, std::move(right), op.line
        );
    }

    return expr;
}

std::unique_ptr<Expr> Parser::parse_factor()
{
    auto expr = parse_unary();

    while (match({TokenType::Slash, TokenType::Star}))
    {
        Token op = previous();
        auto right = parse_unary();
        expr = std::make_unique<BinaryExpr>(
            std::move(expr), op, std::move(right), op.line
        );
    }

    return expr;
}

std::unique_ptr<Expr> Parser::parse_unary()
{
    if (match({TokenType::Bang, TokenType::Minus}))
    {
        Token op = previous();
        auto right = parse_unary();
        return std::make_unique<UnaryExpr>(op, std::move(right), op.line);
    }

    return parse_primary();
}

std::unique_ptr<Expr> Parser::parse_primary()
{
    if (match({TokenType::Number, TokenType::String}))
    {
        Token token = previous();
        return std::make_unique<LiteralExpr>(token.literal, token.line);
    }

    if (match({TokenType::LeftParen}))
    {
        Token paren = previous();  // Save left paren for line number
        auto expr = parse_expr();
        consume(TokenType::RightParen, "Expect ')' after expression");
        return std::make_unique<GroupingExpr>(std::move(expr), paren.line);
    }

    throw std::runtime_error(
        std::to_string(peek().line) + "Expected expression"
    );
}

void Parser::synchronize()
{
    advance();

    while (!is_at_end())
    {
        if (previous().kind == TokenType::SemiColon)
            return;

        switch (peek().kind)
        {
        case TokenType::Struct:
        case TokenType::Fn:
        case TokenType::Let:
        case TokenType::For:
        case TokenType::If:
        case TokenType::While:
        case TokenType::Print:
        case TokenType::Return:
            return;
        default:
            return;
        }

        advance();
    }
}

bool Parser::match(std::vector<TokenType> types)
{
    for (TokenType type : types)
    {
        if (check(type))
        {
            advance();
            return true;
        }
    }

    return false;
}

bool Parser::check(TokenType type)
{
    if (is_at_end())
    {
        return false;
    }
    return peek().kind == type;
}

Token Parser::advance()
{
    if (!is_at_end())
        current++;
    return previous();
}

bool Parser::is_at_end() { return peek().kind == TokenType::Eof; }

Token Parser::peek() { return tokens[current]; }

Token Parser::previous() { return tokens[current - 1]; }

Token Parser::consume(TokenType type, std::string message)
{
    if (check(type))
        return advance();

    throw std::runtime_error(message);
}
