#include "parser.hpp"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "common/expression.hpp"
#include "common/statements.hpp"
#include "common/token.hpp"

Parser::Parser(std::vector<Token> tokens)
    : tokens(std::move(tokens)), current(0)
{
}

Parser::~Parser() {}

std::vector<std::unique_ptr<Stmt>> Parser::parse()
{
    std::vector<std::unique_ptr<Stmt>> statements;
    while (!is_at_end())
    {
        auto stmt = parse_decl();
        if (stmt)
        {
            statements.push_back(std::move(stmt));
        }
    }
    return statements;
}

std::unique_ptr<Stmt> Parser::parse_decl()
{
    try
    {
        if (match({TokenType::Let}))
        {
            return parse_var_decl();
        }

        return parse_stmt();
    }
    catch (std::runtime_error const &err)
    {
        synchronize();
        std::cout << "synchronize...\n";
        return nullptr;
    }
}

std::unique_ptr<Stmt> Parser::parse_var_decl()
{
    Token token = consume(TokenType::Identifier, "Expect variable name.");

    std::unique_ptr<Expr> initializer;
    if (match({TokenType::Equal}))
    {
        initializer = parse_expr();
    }

    consume(TokenType::SemiColon, "Expect ';' after variable declaration.");
    auto stmt = std::make_unique<VarStmt>(
        VarStmt(std::move(token), std::move(initializer))
    );
    return stmt;
}

std::unique_ptr<Stmt> Parser::parse_stmt()
{
    if (match({TokenType::Print}))
    {
        return parse_print_stmt();
    }

    return parse_expr_stmt();
}

std::unique_ptr<Stmt> Parser::parse_print_stmt()
{
    auto value = parse_expr();
    consume(TokenType::SemiColon, "Expect ';' after value.");
    auto stmt = std::make_unique<PrintStmt>(PrintStmt(std::move(value)));
    return stmt;
}

std::unique_ptr<Stmt> Parser::parse_expr_stmt()
{
    auto expr = parse_expr();
    consume(TokenType::SemiColon, "Expect ';' after expression.");
    auto stmt = std::make_unique<ExprStmt>(ExprStmt(std::move(expr)));
    return stmt;
}

std::unique_ptr<Expr> Parser::parse_assignment()
{
    auto expr = parse_equality();

    if (match({TokenType::Equal}))
    {
        Token equals = previous();
        auto value = parse_assignment();

        if (auto *var_expr = dynamic_cast<VarExpr *>(expr.get()))
        {
            Token name = var_expr->token;
            return std::make_unique<AssignExpr>(
                name, std::move(value), name.line
            );
        }

        // TODO: error
        // error(equals, "Invalid assignment target.");
    }

    return expr;
}

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
        return std::make_unique<LiteralExpr>(token, token.line);
    }

    if (match({TokenType::Identifier}))
    {
        Token token = previous();
        return std::make_unique<VarExpr>(token, token.line);
    }

    if (match({TokenType::LeftParen}))
    {
        Token paren = previous();  // Save left paren for line number
        auto expr = parse_expr();
        consume(
            TokenType::RightParen, "line " + std::to_string(paren.line) +
                                       ": Expect ')' after expression"
        );
        return std::make_unique<GroupingExpr>(std::move(expr), paren.line);
    }

    throw std::runtime_error(
        "line " + std::to_string(peek().line) + ": Expected expression"
    );
}

void Parser::synchronize()
{
    advance();

    while (!is_at_end())
    {
        if (previous().kind == TokenType::SemiColon)
        {
            return;
        }

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
    {
        current++;
    }
    return previous();
}

bool Parser::is_at_end() { return peek().kind == TokenType::Eof; }

Token Parser::peek() { return tokens[current]; }

Token Parser::previous() { return tokens[current - 1]; }

Token Parser::consume(TokenType type, std::string message)
{
    if (check(type))
    {
        return advance();
    }

    throw std::runtime_error(message);
}
