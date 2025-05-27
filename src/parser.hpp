#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include "common/token.hpp"

class Parser
{
   public:
    Parser(std::vector<Token> tokens);
    ~Parser();

    void parse();

   private:
    std::vector<Token> tokens;

   private:
    // member functions
};

#endif
