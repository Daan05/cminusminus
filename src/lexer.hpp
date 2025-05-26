#ifndef LEXER_HPP
#define LEXER_HPP

#include "token.hpp"

#include <cstddef>
#include <string>
#include <vector>

class Lexer
{
   public:
    Lexer(std::string const &source);
    ~Lexer();

    void lex();

   private:
    std::string source;
    std::vector<Token> tokens = {};
    size_t start;
    size_t m_current;
    size_t m_line;
    size_t m_column;
 
   private:
    void add_num_token();
};

#endif
