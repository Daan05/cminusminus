#ifndef CODEGENERATOR_HPP
#define CODEGENERATOR_HPP

#include <memory>
#include <sstream>
#include <vector>

#include "common/statements.hpp"

class CodeGenerator
{
   public:
    CodeGenerator(std::vector<std::unique_ptr<Stmt>> statements);
    ~CodeGenerator();

    std::string generate();

   private:
    std::vector<std::unique_ptr<Stmt>> m_statements;
    std::ostringstream m_output;

   private:
    // member function
};

#endif
