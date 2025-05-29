#ifndef PARSER_HPP
#define PARSER_HPP

#include <cstddef>
#include <vector>
#include "common/expression.hpp"
#include "common/token.hpp"

class Parser
{
   public:
    Parser(std::vector<Token> tokens);
    ~Parser();

    Expr parse();

   private:
    std::vector<Token> tokens;
    size_t current;

   private:
    Expr parse_expr();
    Expr parse_equality();
    Expr parse_comparison();
    Expr parse_term();
    Expr parse_factor();
    Expr parse_unary();
    Expr parse_primary();

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
