#include "irinstructions.hpp"

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

UnaryOpIRInstr::UnaryOpIRInstr(
    std::string dst, std::string op, std::string value
)
    : dst(dst), op(op), value(value)
{
}

GotoIRInstr::GotoIRInstr(std::string label) : label(label) {}

IfFalseGotoIRInstr::IfFalseGotoIRInstr(std::string condition, std::string label)
    : condition(condition), label(label)
{
}

LabelIRInstr::LabelIRInstr(std::string name) : name(name) {}

PrintIRInstr::PrintIRInstr(std::string value) : value(value) {}

IRInstr::IRInstr(AssignIRInstr &&instr) : kind(IRInstrType::Assign)
{
    new (&variant.assign) AssignIRInstr(std::move(instr));
}

IRInstr::IRInstr(BinaryOpIRInstr &&instr) : kind(IRInstrType::BinaryOp)
{
    new (&variant.binaryOp) BinaryOpIRInstr(std::move(instr));
}

IRInstr::IRInstr(UnaryOpIRInstr &&instr) : kind(IRInstrType::UnaryOp)
{
    new (&variant.unaryOp) UnaryOpIRInstr(std::move(instr));
}

IRInstr::IRInstr(GotoIRInstr &&instr) : kind(IRInstrType::Goto)
{
    new (&variant.goto_) GotoIRInstr(std::move(instr));
}

IRInstr::IRInstr(IfFalseGotoIRInstr &&instr) : kind(IRInstrType::IfFalseGoto)
{
    new (&variant.ifFalseGoto) IfFalseGotoIRInstr(std::move(instr));
}

IRInstr::IRInstr(LabelIRInstr &&instr) : kind(IRInstrType::Label)
{
    new (&variant.label) LabelIRInstr(std::move(instr));
}

IRInstr::IRInstr(PrintIRInstr &&instr) : kind(IRInstrType::Print)
{
    new (&variant.print) PrintIRInstr(std::move(instr));
}

IRInstr::~IRInstr()
{
    switch (kind)
    {
    case IRInstrType::Assign:
        variant.assign.~AssignIRInstr();
        break;
    case IRInstrType::BinaryOp:
        variant.binaryOp.~BinaryOpIRInstr();
        break;
    case IRInstrType::UnaryOp:
        variant.unaryOp.~UnaryOpIRInstr();
        break;
    case IRInstrType::Goto:
        variant.goto_.~GotoIRInstr();
        break;
    case IRInstrType::IfFalseGoto:
        variant.ifFalseGoto.~IfFalseGotoIRInstr();
        break;
    case IRInstrType::Label:
        variant.label.~LabelIRInstr();
        break;
    case IRInstrType::Print:
        variant.print.~PrintIRInstr();
        break;
    }
}
