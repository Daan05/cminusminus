#ifndef LEXER_HPP
#define LEXER_HPP

#include "common/token.hpp"

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

class Lexer
{
   public:
    Lexer(std::string const &source);
    ~Lexer();

    std::vector<Token> lex();

   private:
    bool m_had_error;
    std::string m_source;
    std::vector<Token> m_tokens = {};
    size_t m_start;
    size_t m_current;
    size_t m_line;
    size_t m_line_start;

    std::unordered_map<std::string, TokenType> keywords;

   private:
    void lex_token();
    void add_token(TokenType kind);
    void add_num_token();
    void add_str_token();
    void add_ident_token();

    bool is_at_end();
    char advance();
    bool match(char expected);
    char peek();
    char peek_next();
};

#endif
