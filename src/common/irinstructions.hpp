#ifndef COMMON_IR_INSTRUCTIONS_HPP
#define COMMON_IR_INSTRUCTIONS_HPP

#include <string>

enum class IRInstrType
{
    Assign,
    BinaryOp,
    UnaryOp,
    Goto,
    IfFalseGoto,
    Label,
    Print,
};

struct IRInstr;

struct AssignIRInstr
{
    AssignIRInstr(std::string dst, std::string src);
    AssignIRInstr(AssignIRInstr &&instr) = default;
    ~AssignIRInstr() = default;

    std::string dst;
    std::string src;
};

struct BinaryOpIRInstr
{
    BinaryOpIRInstr(
        std::string dst, std::string left, std::string op, std::string right
    );
    BinaryOpIRInstr(BinaryOpIRInstr &&instr) = default;
    ~BinaryOpIRInstr() = default;

    std::string dst;
    std::string left;
    std::string op;
    std::string right;
};

struct UnaryOpIRInstr
{
    UnaryOpIRInstr(std::string dst, std::string op, std::string val);
    UnaryOpIRInstr(UnaryOpIRInstr &&instr) = default;
    ~UnaryOpIRInstr() = default;

    std::string dst;
    std::string op;
    std::string value;
};

struct GotoIRInstr
{
    GotoIRInstr(std::string label);
    GotoIRInstr(GotoIRInstr &&instr) = default;
    ~GotoIRInstr() = default;

    std::string label;
};

struct IfFalseGotoIRInstr
{
    IfFalseGotoIRInstr(std::string condition, std::string label);
    IfFalseGotoIRInstr(IfFalseGotoIRInstr &&instr) = default;
    ~IfFalseGotoIRInstr() = default;

    std::string condition;
    std::string label;
};

struct LabelIRInstr
{
    LabelIRInstr(std::string label);
    LabelIRInstr(LabelIRInstr &&instr) = default;
    ~LabelIRInstr() = default;

    std::string name;
};

struct PrintIRInstr
{
    PrintIRInstr(std::string label);
    PrintIRInstr(PrintIRInstr &&instr) = default;
    ~PrintIRInstr() = default;

    std::string value;
};
struct IRInstr
{
    IRInstr(AssignIRInstr &&instr);
    IRInstr(BinaryOpIRInstr &&instr);
    IRInstr(UnaryOpIRInstr &&instr);
    IRInstr(GotoIRInstr &&instr);
    IRInstr(IfFalseGotoIRInstr &&instr);
    IRInstr(LabelIRInstr &&instr);
    IRInstr(PrintIRInstr &&instr);
    ~IRInstr();

    IRInstrType kind;
    union Variant
    {
        AssignIRInstr assign;
        BinaryOpIRInstr binaryOp;
        UnaryOpIRInstr unaryOp;
        GotoIRInstr goto_;
        IfFalseGotoIRInstr ifFalseGoto;
        LabelIRInstr label;
        PrintIRInstr print;

        Variant() {}
        ~Variant() {}
    } variant;
};

#endif
