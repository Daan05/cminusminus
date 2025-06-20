#include "irinstructions.hpp"

#include "error.hpp"

#include <string>

AssignIR::AssignIR(std::string dst, std::string src) : dst(dst), src(src) {}

BinaryOpIR::BinaryOpIR(
    std::string dst, std::string left, std::string op, std::string right
)
    : dst(dst), left(left), op(op), right(right)
{
}

UnaryOpIR::UnaryOpIR(std::string dst, std::string op, std::string value)
    : dst(dst), op(op), value(value)
{
}

GotoIR::GotoIR(std::string label) : label(label) {}

IfFalseGotoIR::IfFalseGotoIR(std::string condition, std::string label)
    : condition(condition), label(label)
{
}

LabelIR::LabelIR(std::string name) : name(name) {}

PrintIR::PrintIR(std::string value) : value(value) {}

CallIR::CallIR(
    std::string dst, std::string funcName, std::vector<std::string> args
)
    : dst(dst), funcName(funcName), args(args)
{
}

ReturnIR::ReturnIR(std::string value) : value(value) {}

IRInstr::IRInstr(AssignIR &&instr) : kind(IRType::Assign)
{
    new (&variant.assign) AssignIR(std::move(instr));
}

IRInstr::IRInstr(BinaryOpIR &&instr) : kind(IRType::BinaryOp)
{
    new (&variant.binaryOp) BinaryOpIR(std::move(instr));
}

IRInstr::IRInstr(UnaryOpIR &&instr) : kind(IRType::UnaryOp)
{
    new (&variant.unaryOp) UnaryOpIR(std::move(instr));
}

IRInstr::IRInstr(GotoIR &&instr) : kind(IRType::Goto)
{
    new (&variant.goto_) GotoIR(std::move(instr));
}

IRInstr::IRInstr(IfFalseGotoIR &&instr) : kind(IRType::IfFalseGoto)
{
    new (&variant.ifFalseGoto) IfFalseGotoIR(std::move(instr));
}

IRInstr::IRInstr(LabelIR &&instr) : kind(IRType::Label)
{
    new (&variant.label) LabelIR(std::move(instr));
}

IRInstr::IRInstr(PrintIR &&instr) : kind(IRType::Print)
{
    new (&variant.print) PrintIR(std::move(instr));
}

IRInstr::IRInstr(CallIR &&instr) : kind(IRType::Call)
{
    new (&variant.call) CallIR(std::move(instr));
}

IRInstr::IRInstr(ReturnIR &&instr) : kind(IRType::Return)
{
    new (&variant.return_) ReturnIR(std::move(instr));
}

IRInstr::~IRInstr()
{
    switch (kind)
    {
    case IRType::Assign:
        variant.assign.~AssignIR();
        break;
    case IRType::BinaryOp:
        variant.binaryOp.~BinaryOpIR();
        break;
    case IRType::UnaryOp:
        variant.unaryOp.~UnaryOpIR();
        break;
    case IRType::Goto:
        variant.goto_.~GotoIR();
        break;
    case IRType::IfFalseGoto:
        variant.ifFalseGoto.~IfFalseGotoIR();
        break;
    case IRType::Label:
        variant.label.~LabelIR();
        break;
    case IRType::Print:
        variant.print.~PrintIR();
        break;
    case IRType::Call:
        variant.call.~CallIR();
        break;
    case IRType::Return:
        variant.return_.~ReturnIR();
        break;
    }
}

std::string IRInstr::to_string()
{
    switch (kind)
    {
    case IRType::Assign:
    {
        auto &ir = variant.assign;
        return ir.dst + " = " + ir.src;
    }
    case IRType::BinaryOp:
    {
        auto &ir = variant.binaryOp;
        return ir.dst + " = " + ir.left + " " + ir.op + " " + ir.right;
    }
    case IRType::UnaryOp:
    {
        auto &ir = variant.unaryOp;
        return ir.dst + " = " + ir.op + " " + ir.value;
    }
    case IRType::Goto:
    {
        auto &ir = variant.goto_;
        return "goto " + ir.label;
    }
    case IRType::IfFalseGoto:
    {
        auto &ir = variant.ifFalseGoto;
        return "ifFalse " + ir.condition + " goto " + ir.label;
    }
    case IRType::Label:
    {
        auto &ir = variant.label;
        return ir.name + ":";
    }
    case IRType::Print:
    {
        auto &ir = variant.print;
        return "print " + ir.value;
    }
    case IRType::Call:
    {
        auto &ir = variant.call;
        std::string str = ir.funcName + "(";
        for (auto const &arg : ir.args)
        {
            str += arg + " ";
        }
        str += ")";
        return str;
    }
    case IRType::Return:
    {
        auto &ir = variant.return_;
        return "return " + ir.value;
    }
    }

    error::unreachable();
    return "";
}
