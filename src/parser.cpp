#include "parser.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "common/expression.hpp"
#include "common/error.hpp"
#include "common/statements.hpp"
#include "common/token.hpp"

static int rbp_offset = 8;

Parser::Parser(std::vector<Token> tokens)
    : m_had_error(false),
      m_tokens(std::move(tokens)),
      m_current(0),
      m_scope_depth(0)
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

    if (m_had_error)
    {
        error::fatal("Encountered an error during parsing pass");
    }
    return statements;
}

std::unique_ptr<Stmt> Parser::parse_decl()
try
{
    if (match({TokenType::Let}))
    {
        return parse_var_decl();
    }

    return parse_stmt();
}
catch (error::Synchronize const &err)
{
    m_had_error = true;
    synchronize();
    return nullptr;
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
        VarStmt(LocalVar(token, m_scope_depth), std::move(initializer))
    );
    rbp_offset += 8;
    return stmt;
}

std::unique_ptr<Stmt> Parser::parse_stmt()
{
    if (match({TokenType::Print}))
    {
        return parse_print_stmt();
    }
    else if (match({TokenType::LeftBrace}))
    {
        return parse_block_stmt();
    }
    else if (match({TokenType::If}))
    {
        return parse_if_stmt();
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

std::unique_ptr<Stmt> Parser::parse_block_stmt()
{
    // begin scope
    m_scope_depth++;

    std::vector<std::unique_ptr<Stmt>> statements = {};
    while (!check(TokenType::RightBrace) && !is_at_end())
    {
        statements.push_back(parse_decl());
    }
    consume(TokenType::RightBrace, "Expect '}' after block.");

    // end scope
    m_scope_depth--;

    std::unique_ptr<BlockStmt> stmt =
        std::make_unique<BlockStmt>(BlockStmt(std::move(statements)));
    return stmt;
}

std::unique_ptr<Stmt> Parser::parse_expr_stmt()
{
    auto expr = parse_expr();
    consume(TokenType::SemiColon, "Expect ';' after expression.");
    auto stmt = std::make_unique<ExprStmt>(ExprStmt(std::move(expr)));
    return stmt;
}

std::unique_ptr<Stmt> Parser::parse_if_stmt()
{
    consume(TokenType::LeftParen, "Expect '(' after 'if'");
    auto condition = parse_expr();
    consume(TokenType::RightParen, "Expect ')' after if condition");

    auto thenBranch = parse_stmt();
    std::unique_ptr<Stmt> elseBranch = nullptr;
    if (match({TokenType::Else}))
    {
        elseBranch = parse_stmt();
    }

    auto stmt = std::make_unique<IfStmt>(IfStmt(
        std::move(condition), std::move(thenBranch), std::move(elseBranch)
    ));
    return stmt;
}

std::unique_ptr<Expr> Parser::parse_expr() { return parse_assignment(); }

std::unique_ptr<Expr> Parser::parse_assignment()
{
    auto expr = parse_or();

    if (match({TokenType::Equal}))
    {
        Token equals = previous();
        auto value = parse_assignment();

        if (auto *var_expr = dynamic_cast<VarExpr *>(expr.get()))
        {
            Token name = var_expr->var->token;
            return std::make_unique<AssignExpr>(
                LocalVar(name, m_scope_depth), std::move(value), name.line
            );
        }

        error::synchronize(previous().line, "Invalid assignment target.");
    }

    return expr;
}

std::unique_ptr<Expr> Parser::parse_or()
{
    auto left = parse_and();

    while (match({TokenType::BoolOr}))
    {
        Token op = previous();
        auto right = parse_and();
        auto expr = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right), op.line);
        return expr;
    }

    return left;
}

std::unique_ptr<Expr> Parser::parse_and()
{
    auto left = parse_equality();

    while (match({TokenType::BoolAnd}))
    {
        Token op = previous();
        auto right = parse_equality();
        auto expr = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right), op.line);
        return expr;
    }

    return left;
}

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

        return std::make_unique<VarExpr>(token, m_scope_depth, token.line);
    }

    if (match({TokenType::LeftParen}))
    {
        Token paren = previous();  // Save left paren for line number
        auto expr = parse_expr();
        consume(TokenType::RightParen, "Expect ')' after expression");
        return std::make_unique<GroupingExpr>(std::move(expr), paren.line);
    }

    error::synchronize(previous().line, "Expected expression");
    return nullptr;
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
        m_current++;
    }
    return previous();
}

bool Parser::is_at_end() { return peek().kind == TokenType::Eof; }

Token Parser::peek() { return m_tokens[m_current]; }

Token Parser::previous() { return m_tokens[m_current - 1]; }

Token Parser::consume(TokenType type, std::string message)
{
    if (check(type))
    {
        return advance();
    }

    error::synchronize(previous().line, message);
    return Token();
}
