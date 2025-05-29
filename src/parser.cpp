#include "parser.hpp"
#include <format>
#include <stdexcept>

#include "common/expression.hpp"
#include "common/token.hpp"

Parser::Parser(std::vector<Token> tokens) : tokens(tokens), current(0) {}

Parser::~Parser() {}

Expr Parser::parse() { return parse_expr(); }

Expr Parser::parse_expr() { return parse_equality(); }

Expr Parser::parse_equality()
{
    Expr expr = parse_comparison();

    while (match({TokenType::BangEqual, TokenType::EqualEqual}))
    {
        Token op = previous();
        Expr right = parse_comparison();
        expr = BinaryExpr(expr, op, right, 1);
    }

    return expr;
}

Expr Parser::parse_comparison()
{
    Expr expr = parse_term();

    while (match(
        {TokenType::Greater, TokenType::GreaterEqual, TokenType::Less,
         TokenType::LessEqual}
    ))
    {
        Token op = previous();
        Expr right = parse_term();
        expr = BinaryExpr(expr, op, right, 1);
    }

    return expr;
}

Expr Parser::parse_term()
{
    Expr expr = parse_factor();

    while (match({TokenType::Plus, TokenType::Minus}))
    {
        Token op = previous();
        Expr right = parse_factor();
        expr = BinaryExpr(expr, op, right, 1);
    }

    return expr;
}

Expr Parser::parse_factor()
{
    Expr expr = parse_unary();

    while (match({TokenType::Slash, TokenType::Star}))
    {
        Token op = previous();
        Expr right = parse_unary();
        expr = BinaryExpr(expr, op, right, 1);
    }

    return expr;
}

Expr Parser::parse_unary()
{
    if (match({TokenType::Bang, TokenType::Minus}))
    {
        Token op = previous();
        Expr right = parse_unary();
        return UnaryExpr(op, right, 1);
    }

    return parse_primary();
}

Expr Parser::parse_primary()
{
    if (match({TokenType::Number, TokenType::String}))
    {
        return LiteralExpr(previous().literal, 1);
    }

    if (match({TokenType::LeftParen}))
    {
        Expr expr = parse_expr();
        // consume(TokenType::RightParen, "Expect ')' after expression.");
        return GroupingExpr(expr, 1);
    }

    throw std::runtime_error(
        std::format("line %d: %s", peek().line, "Expected expression")
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

    throw std::runtime_error(std::format("line %d: %s", peek().line, message));
    ;
}
