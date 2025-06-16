#ifndef COMMON_IR_INSTRUCTIONS_HPP
#define COMMON_IR_INSTRUCTIONS_HPP

#include <string>

enum class IRType
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

struct AssignIR
{
    AssignIR(std::string dst, std::string src);
    AssignIR(AssignIR &&instr) = default;
    ~AssignIR() = default;

    std::string dst;
    std::string src;
};

struct BinaryOpIR
{
    BinaryOpIR(
        std::string dst, std::string left, std::string op, std::string right
    );
    BinaryOpIR(BinaryOpIR &&instr) = default;
    ~BinaryOpIR() = default;

    std::string dst;
    std::string left;
    std::string op;
    std::string right;
};

struct UnaryOpIR
{
    UnaryOpIR(std::string dst, std::string op, std::string val);
    UnaryOpIR(UnaryOpIR &&instr) = default;
    ~UnaryOpIR() = default;

    std::string dst;
    std::string op;
    std::string value;
};

struct GotoIR
{
    GotoIR(std::string label);
    GotoIR(GotoIR &&instr) = default;
    ~GotoIR() = default;

    std::string label;
};

struct IfFalseGotoIR
{
    IfFalseGotoIR(std::string condition, std::string label);
    IfFalseGotoIR(IfFalseGotoIR &&instr) = default;
    ~IfFalseGotoIR() = default;

    std::string condition;
    std::string label;
};

struct LabelIR
{
    LabelIR(std::string label);
    LabelIR(LabelIR &&instr) = default;
    ~LabelIR() = default;

    std::string name;
};

struct PrintIR
{
    PrintIR(std::string label);
    PrintIR(PrintIR &&instr) = default;
    ~PrintIR() = default;

    std::string value;
};
struct IRInstr
{
    IRInstr(AssignIR &&instr);
    IRInstr(BinaryOpIR &&instr);
    IRInstr(UnaryOpIR &&instr);
    IRInstr(GotoIR &&instr);
    IRInstr(IfFalseGotoIR &&instr);
    IRInstr(LabelIR &&instr);
    IRInstr(PrintIR &&instr);
    ~IRInstr();

    IRType kind;
    union Variant
    {
        AssignIR assign;
        BinaryOpIR binaryOp;
        UnaryOpIR unaryOp;
        GotoIR goto_;
        IfFalseGotoIR ifFalseGoto;
        LabelIR label;
        PrintIR print;

        Variant() {}
        ~Variant() {}
    } variant;

    std::string to_string();
};

#endif
