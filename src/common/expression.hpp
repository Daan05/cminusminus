#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <memory>

#include "token.hpp"

class ExprVisitor
{
   public:
    virtual ~ExprVisitor() = default;
    virtual void visit_binary_expr(class BinaryExpr const &expr) = 0;
    virtual void visit_literal_expr(class LiteralExpr const &expr) = 0;
    virtual void visit_unary_expr(class UnaryExpr const &expr) = 0;
    virtual void visit_grouping_expr(class GroupingExpr const &expr) = 0;
};

class Expr
{
   public:
    virtual ~Expr() = default;
    int get_line() const;
    virtual void accept(ExprVisitor &visitor) const = 0;

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

    void accept(ExprVisitor &visitor) const override;

    std::unique_ptr<Expr> m_left;
    Token m_op;
    std::unique_ptr<Expr> m_right;
};

class LiteralExpr : public Expr
{
   public:
    LiteralExpr(Literal literal, int line);

    void accept(ExprVisitor &visitor) const override;

    Literal m_literal;
};

class UnaryExpr : public Expr
{
   public:
    UnaryExpr(Token op, std::unique_ptr<Expr> right, int line);

    void accept(ExprVisitor &visitor) const override;

    Token m_op;
    std::unique_ptr<Expr> m_right;
};

class GroupingExpr : public Expr
{
   public:
    GroupingExpr(std::unique_ptr<Expr> expr, int line);

    void accept(ExprVisitor &visitor) const override;

    std::unique_ptr<Expr> m_expr;
};

#endif
