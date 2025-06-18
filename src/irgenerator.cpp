#include "irgenerator.hpp"

#include <memory>
#include <vector>

#include "common/error.hpp"

std::pair<
    std::vector<std::unique_ptr<IRInstr>>,
    std::unordered_map<std::string, IRFunction>>
IRGenerator::generate(std::vector<std::unique_ptr<Stmt>> const &statements)
{
    for (auto const &stmt : statements)
    {
        lower_stmt(*stmt);
    }

    return {std::move(m_context.main), std::move(m_context.functions)};
}

std::string IRGenerator::lower_expr(Expr const &expr)
{
    switch (expr.kind)
    {
    case ExprType::Literal:
    {
        auto &lit = expr.variant.literal;
        std::string temp = m_context.new_temp();
        m_context.emit_main(
            std::make_unique<IRInstr>(AssignIR(temp, lit.token.lexeme))
        );
        return temp;
    }
    case ExprType::Var:
    {
        auto &var = expr.variant.var;
        return var.var.token.lexeme;
    }
    case ExprType::Assign:
    {
        auto &assign = expr.variant.assign;
        std::string rhs = lower_expr(*assign.expr);
        std::string name = assign.var.token.lexeme;
        m_context.emit_main(std::make_unique<IRInstr>(AssignIR(name, rhs)));
        return name;
    }
    case ExprType::Binary:
    {
        auto &bin = expr.variant.binary;
        std::string lhs = lower_expr(*bin.left);
        std::string rhs = lower_expr(*bin.right);
        std::string temp = m_context.new_temp();
        m_context.emit_main(
            std::make_unique<IRInstr>(BinaryOpIR(temp, lhs, bin.op.lexeme, rhs))
        );
        return temp;
    }
    case ExprType::Unary:
    {
        auto &un = expr.variant.unary;
        std::string val = lower_expr(*un.expr);
        std::string temp = m_context.new_temp();
        m_context.emit_main(
            std::make_unique<IRInstr>(UnaryOpIR(temp, un.op.lexeme, val))
        );
        return temp;
    }
    case ExprType::Grouping:
    {
        return lower_expr(*expr.variant.grouping.expr);
    }
    case ExprType::Call:
    {
        auto &call = expr.variant.call;

        std::string func = lower_expr(*expr.variant.call.callee);

        std::vector<std::string> args;
        for (auto &arg : call.args)
        {
            args.push_back(lower_expr(*arg));
        }

        std::string dst = m_context.new_temp();
        m_context.emit_main(std::make_unique<IRInstr>(CallIR(dst, func, args)));
        return dst;
        break;
    }
    }

    error::unreachable();
    return "";
}

void IRGenerator::lower_stmt(Stmt const &stmt)
{
    switch (stmt.kind)
    {
    case StmtType::Expr:
    {
        lower_expr(*stmt.variant.expr.expr);
        break;
    }
    case StmtType::Print:
    {
        std::string val = lower_expr(*stmt.variant.print.expr);
        m_context.emit_main(std::make_unique<IRInstr>(PrintIR(val)));
        break;
    }
    case StmtType::Var:
    {
        std::string val = lower_expr(*stmt.variant.var.expr);
        std::string name = stmt.variant.var.var.token.lexeme;
        m_context.emit_main(std::make_unique<IRInstr>(AssignIR(name, val)));
        break;
    }
    case StmtType::Block:
    {
        for (auto &sub : stmt.variant.block.statements)
        {
            lower_stmt(*sub);
        }
        break;
    }
    case StmtType::If:
    {
        auto &if_ = stmt.variant.if_;
        std::string cond = lower_expr(*if_.condition);
        std::string elseLabel = m_context.new_label();
        std::string endLabel = m_context.new_label();

        m_context.emit_main(
            std::make_unique<IRInstr>(IfFalseGotoIR(cond, elseLabel))
        );
        lower_stmt(*if_.then_branch);
        m_context.emit_main(std::make_unique<IRInstr>(GotoIR(endLabel)));
        m_context.emit_main(std::make_unique<IRInstr>(LabelIR(elseLabel)));
        if (if_.else_branch)
            lower_stmt(*if_.else_branch.get());
        m_context.emit_main(std::make_unique<IRInstr>(LabelIR(endLabel)));
        break;
    }
    case StmtType::While:
    {
        auto &wh = stmt.variant.while_;
        std::string startLabel = m_context.new_label();
        std::string endLabel = m_context.new_label();

        m_context.emit_main(std::make_unique<IRInstr>(LabelIR(startLabel)));
        std::string cond = lower_expr(*wh.condition);
        m_context.emit_main(
            std::make_unique<IRInstr>(IfFalseGotoIR(cond, endLabel))
        );
        lower_stmt(*wh.body.get());
        m_context.emit_main(std::make_unique<IRInstr>(GotoIR(startLabel)));
        m_context.emit_main(std::make_unique<IRInstr>(LabelIR(endLabel)));
        break;
    }
    case StmtType::Func:
    {
        auto &fn = stmt.variant.func;
        std::string funcLabel = "func_" + fn.name.lexeme;

        auto old_main = std::move(m_context.main);
        int old_temp = m_context.temp_count;
        int old_label = m_context.label_count;

        m_context.main.clear();
        m_context.temp_count = 0;
        m_context.label_count = 0;

        m_context.emit_main(std::make_unique<IRInstr>(LabelIR(funcLabel)));
        lower_stmt(*fn.body);
        m_context.emit_main(std::make_unique<IRInstr>(ReturnIR("")));

        for (auto const &param : fn.params)
        {
            m_context.functions[fn.name.lexeme].params.push_back(param.lexeme);
        }

        m_context.functions[fn.name.lexeme].body = std::move(m_context.main);

        m_context.main = std::move(old_main);
        m_context.temp_count = old_temp;
        m_context.label_count = old_label;

        break;
    }
    }
}
