#ifndef PARSER_HPP
#define PARSER_HPP

#include <cstddef>
#include <memory>
#include <vector>
#include "common/expression.hpp"
#include "common/statements.hpp"
#include "common/token.hpp"

class Parser
{
   public:
    Parser(std::vector<Token> tokens);
    ~Parser();

    std::vector<std::unique_ptr<Stmt>> parse();

   private:
    std::vector<Token> tokens;
    size_t current;

   private:
    std::unique_ptr<Stmt> parse_decl();
    std::unique_ptr<Stmt> parse_var_decl();

    std::unique_ptr<Stmt> parse_stmt();
    std::unique_ptr<Stmt> parse_print_stmt();
    std::unique_ptr<Stmt> parse_expr_stmt();

    std::unique_ptr<Expr> parse_assignment();
    std::unique_ptr<Expr> parse_expr();
    std::unique_ptr<Expr> parse_equality();
    std::unique_ptr<Expr> parse_comparison();
    std::unique_ptr<Expr> parse_term();
    std::unique_ptr<Expr> parse_factor();
    std::unique_ptr<Expr> parse_unary();
    std::unique_ptr<Expr> parse_primary();

    void synchronize();

    bool match(std::vector<TokenType> types);
    bool check(TokenType type);
    Token advance();
    bool is_at_end();
    Token peek();
    Token previous();
    Token consume(TokenType type, std::string message);
};

#endif
