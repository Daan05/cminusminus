#include "statements.hpp"

#include <memory>

ExprStmt::ExprStmt(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}

PrintStmt::PrintStmt(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}

VarStmt::VarStmt(LocalVar var, std::unique_ptr<Expr> expr)
    : var(std::move(var)), expr(std::move(expr))
{
}

BlockStmt::BlockStmt(std::vector<std::unique_ptr<Stmt>> statements)
    : statements(std::move(statements))
{
}

IfStmt::IfStmt(
    std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> then_branch,
    std::unique_ptr<Stmt> else_branch
)
    : condition(std::move(condition)),
      then_branch(std::move(then_branch)),
      else_branch(std::move(else_branch))
{
}

WhileStmt::WhileStmt(
    std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body
)
    : condition(std::move(condition)), body(std::move(body))
{
}
FuncStmt::FuncStmt(
    Token &&name, std::vector<Token> params, std::unique_ptr<Stmt> body
)
    : name(std::move(name)), params(std::move(params)), body(std::move(body))
{
}

Stmt::Stmt(size_t line, ExprStmt &&expr) : line(line), kind(StmtType::Expr)
{
    new (&variant.expr) ExprStmt(std::move(expr));
}

Stmt::Stmt(size_t line, PrintStmt &&expr) : line(line), kind(StmtType::Print)
{
    new (&variant.print) PrintStmt(std::move(expr));
}

Stmt::Stmt(size_t line, VarStmt &&expr) : line(line), kind(StmtType::Var)
{
    new (&variant.var) VarStmt(std::move(expr));
}

Stmt::Stmt(size_t line, BlockStmt &&expr) : line(line), kind(StmtType::Block)
{
    new (&variant.block) BlockStmt(std::move(expr));
}

Stmt::Stmt(size_t line, IfStmt &&expr) : line(line), kind(StmtType::If)
{
    new (&variant.if_) IfStmt(std::move(expr));
}

Stmt::Stmt(size_t line, WhileStmt &&expr) : line(line), kind(StmtType::While)
{
    new (&variant.while_) WhileStmt(std::move(expr));
}

Stmt::Stmt(size_t line, FuncStmt &&expr) : line(line), kind(StmtType::Func)
{
    new (&variant.func) FuncStmt(std::move(expr));
}

Stmt::~Stmt()
{
    switch (kind)
    {
    case StmtType::Expr:
        variant.expr.~ExprStmt();
        break;
    case StmtType::Print:
        variant.print.~PrintStmt();
        break;
    case StmtType::Var:
        variant.var.~VarStmt();
        break;
    case StmtType::Block:
        variant.block.~BlockStmt();
        break;
    case StmtType::If:
        variant.if_.~IfStmt();
        break;
    case StmtType::While:
        variant.while_.~WhileStmt();
        break;
    case StmtType::Func:
        variant.func.~FuncStmt();
        break;
    }
}
