#ifndef CODEGENERATOR_HPP
#define CODEGENERATOR_HPP

#include <memory>
#include <sstream>
#include <vector>

#include "common/expression.hpp"
#include "common/statements.hpp"

class CodeGenerator
{
   public:
    CodeGenerator(
        std::pair<std::vector<std::unique_ptr<Stmt>>, std::vector<LocalVar>>
            statements
    );
    ~CodeGenerator();

    std::string generate();

   private:
    std::pair<std::vector<std::unique_ptr<Stmt>>, std::vector<LocalVar>>
        m_statements;
    std::ostringstream m_output;

   private:
    // member function
};

#endif
