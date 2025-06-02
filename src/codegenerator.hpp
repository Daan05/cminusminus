#ifndef CODEGENERATOR_HPP
#define CODEGENERATOR_HPP

#include <memory>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "common/statements.hpp"

class CodeGenerator
{
   public:
    CodeGenerator(
        std::pair<
            std::vector<std::unique_ptr<Stmt>>,
            std::unordered_map<std::string, size_t>>
            statements
    );
    ~CodeGenerator();

    std::string generate();

   private:
    std::pair<
        std::vector<std::unique_ptr<Stmt>>,
        std::unordered_map<std::string, size_t>>
        m_statements;
    std::ostringstream m_output;

   private:
    // member function
};

#endif
