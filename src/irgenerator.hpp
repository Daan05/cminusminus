#ifndef IRGENERATOR_HPP
#define IRGENERATOR_HPP

#include <memory>
#include <unordered_map>
#include <vector>

#include "common/expression.hpp"
#include "common/statements.hpp"
#include "common/irinstructions.hpp"

struct IRFunction
{
    std::string name;
    std::vector<std::string> params;
    std::vector<std::unique_ptr<IRInstr>> body;
};

struct IRContext
{
    IRContext() = default;
    IRContext(IRContext &ctx) = default;

    int temp_count = 0;
    int label_count = 0;

    std::vector<std::unique_ptr<IRInstr>> main;
    std::unordered_map<std::string, IRFunction> functions;

    std::string new_temp() { return "t" + std::to_string(temp_count++); }
    std::string new_label() { return "L" + std::to_string(label_count++); }

    void emit_main(std::unique_ptr<IRInstr> instr)
    {
        main.push_back(std::move(instr));
    }

    void emit_func(const std::string &name, std::unique_ptr<IRInstr> instr)
    {
        functions[name].body.push_back(std::move(instr));
    }
};

class IRGenerator
{
   public:
    IRGenerator() : m_context(IRContext()) {};
    ~IRGenerator() = default;

    std::pair<
        std::vector<std::unique_ptr<IRInstr>>,
        std::unordered_map<std::string, IRFunction>>
    generate(std::vector<std::unique_ptr<Stmt>> const &statements);

    std::string lower_expr(Expr const &expr);
    void lower_stmt(Stmt const &stmt);

   private:
    IRContext m_context;
};

#endif
