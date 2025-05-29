#ifndef LEXER_HPP
#define LEXER_HPP

#include "common/token.hpp"

#include <cstddef>
#include <string>
#include <vector>

class Lexer
{
   public:
    Lexer(std::string const &source);
    ~Lexer();

    std::vector<Token> lex();

   private:
    std::string source;
    std::vector<Token> tokens = {};
    size_t m_start;
    size_t m_current;
    size_t m_line;
    size_t m_line_start;

   private:
    void lex_token();
    void add_token(TokenType kind);
    void add_num_token();
    void add_str_token();

    bool is_at_end();
    bool match(char expected);
};

#endif
