#include "irinstructions.hpp"

#include "error.hpp"

#include <string>

AssignIRInstr::AssignIRInstr(std::string dst, std::string src)
    : dst(dst), src(src)
{
}

BinaryOpIRInstr::BinaryOpIRInstr(
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
    }

    error::unreachable();
    return "";
}