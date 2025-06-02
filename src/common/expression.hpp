#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <memory>

#include "token.hpp"

struct Var
{
    Var(Token token, int offset);
    ~Var() = default;

    Token token;
    int rbp_offset;
};

struct ExprVisitor
{
   public:
    virtual ~ExprVisitor() = default;
    virtual void visit_binary_expr(struct BinaryExpr const &expr) = 0;
    virtual void visit_literal_expr(struct LiteralExpr const &expr) = 0;
    virtual void visit_var_decl_expr(struct VarExpr const &expr) = 0;
    virtual void visit_assign_expr(struct AssignExpr const &expr) = 0;
    virtual void visit_unary_expr(struct UnaryExpr const &expr) = 0;
    virtual void visit_grouping_expr(struct GroupingExpr const &expr) = 0;
};

struct Expr
{
   public:
    virtual ~Expr() = default;
    int get_line() const;
    virtual void accept(ExprVisitor &visitor) const = 0;

    int m_line;

   protected:
    Expr(int line);
};

struct BinaryExpr : public Expr
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

struct LiteralExpr : public Expr
{
   public:
    LiteralExpr(Token token, int line);

    void accept(ExprVisitor &visitor) const override;

    Token token;
};

struct VarExpr : public Expr
{
   public:
    VarExpr(Token token, int offset, int line);

    void accept(ExprVisitor &visitor) const override;

    Var var;
};

struct AssignExpr : public Expr
{
   public:
    AssignExpr(Var var, std::unique_ptr<Expr> expr, int line);

    void accept(ExprVisitor &visitor) const override;

    Var var;
    std::unique_ptr<Expr> m_expr;
};

struct UnaryExpr : public Expr
{
   public:
    UnaryExpr(Token op, std::unique_ptr<Expr> right, int line);

    void accept(ExprVisitor &visitor) const override;

    Token m_op;
    std::unique_ptr<Expr> m_right;
};

struct GroupingExpr : public Expr
{
   public:
    GroupingExpr(std::unique_ptr<Expr> expr, int line);

    void accept(ExprVisitor &visitor) const override;

    std::unique_ptr<Expr> m_expr;
};

#endif
