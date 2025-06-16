#ifndef IRGENERATOR_HPP
#define IRGENERATOR_HPP

#include <memory>
#include <vector>

#include "common/expression.hpp"
#include "common/statements.hpp"
#include "common/irinstructions.hpp"

struct IRContext
{
    int temp_count = 0;
    int label_count = 0;
    std::vector<std::unique_ptr<IRInstr>> code;

    std::string new_temp() { return "t" + std::to_string(temp_count++); }

    std::string new_label() { return "L" + std::to_string(label_count++); }

    void emit(std::unique_ptr<IRInstr> instr)
    {
        code.push_back(std::move(instr));
    }
};

class IRGenerator
{
   public:
    IRGenerator() : m_context(IRContext()) {};
    ~IRGenerator() = default;

    std::vector<std::unique_ptr<IRInstr>> generate(
        std::vector<std::unique_ptr<Stmt>> const &statements
    );

    std::string lower_expr(Expr const &expr);
    void lower_stmt(Stmt const &stmt);

   private:
    IRContext m_context;
};

#endif
