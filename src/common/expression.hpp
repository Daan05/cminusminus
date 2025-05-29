#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <memory>
#include <string>

#include "token.hpp"

class Visitor
{
   public:
    virtual ~Visitor() = default;
    virtual void visitBinary(const class BinaryExpr &expr) = 0;
    virtual void visitLiteral(const class LiteralExpr &expr) = 0;
    virtual void visitUnary(const class UnaryExpr &expr) = 0;
    virtual void visitGrouping(const class GroupingExpr &expr) = 0;
};

class Expr
{
   public:
    virtual ~Expr() = default;
    int get_line() const;
    virtual void accept(Visitor &visitor) const = 0;

    int m_line;

   protected:
    Expr(int line);
};

class BinaryExpr : public Expr
{
   public:
    BinaryExpr(
        std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right,
        int line
    );

    void accept(Visitor &visitor) const override;

    std::unique_ptr<Expr> m_left;
    Token m_op;
    std::unique_ptr<Expr> m_right;
};

class LiteralExpr : public Expr
{
   public:
    LiteralExpr(Literal literal, int line);

    void accept(Visitor &visitor) const override;

    Literal m_literal;
};

class UnaryExpr : public Expr
{
   public:
    UnaryExpr(Token op, std::unique_ptr<Expr> right, int line);

    void accept(Visitor &visitor) const override;

    Token m_op;
    std::unique_ptr<Expr> m_right;
};

class GroupingExpr : public Expr
{
   public:
    GroupingExpr(std::unique_ptr<Expr> expr, int line);

    void accept(Visitor &visitor) const override;

    std::unique_ptr<Expr> m_expr;
};

class ASTPrinter : public Visitor
{
   public:
    void print(Expr const &expr);
    void visitBinary(BinaryExpr const &expr) override;
    void visitLiteral(LiteralExpr const &expr) override;
    void visitUnary(UnaryExpr const &expr) override;
    void visitGrouping(GroupingExpr const &expr) override;

   private:
    std::string m_output;
};

class ASTCodeGenerator : public Visitor
{
   public:
    std::string generate(Expr const &expr);
    void visitBinary(BinaryExpr const &expr) override;
    void visitLiteral(LiteralExpr const &expr) override;
    void visitUnary(UnaryExpr const &expr) override;
    void visitGrouping(GroupingExpr const &expr) override;

private:
    std::string m_output;
    size_t int_count;
};

#endif
