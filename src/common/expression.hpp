#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <memory>

#include "token.hpp"

struct LocalVar
{
    LocalVar(Token token, int scope_depth);
    ~LocalVar() = default;

    Token token;
    int scope_depth;
    int rbp_offset;
};

struct ExprVisitor
{
   public:
    virtual ~ExprVisitor() = default;
    virtual void visit_binary_expr(struct BinaryExpr &expr) = 0;
    virtual void visit_literal_expr(struct LiteralExpr &expr) = 0;
    virtual void visit_var_expr(struct VarExpr &expr) = 0;
    virtual void visit_assign_expr(struct AssignExpr &expr) = 0;
    virtual void visit_unary_expr(struct UnaryExpr &expr) = 0;
    virtual void visit_grouping_expr(struct GroupingExpr &expr) = 0;
};

struct Expr
{
   public:
    virtual ~Expr() = default;
    int get_line() const;
    virtual void accept(ExprVisitor &visitor) = 0;

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

    void accept(ExprVisitor &visitor) override;

    std::unique_ptr<Expr> m_left;
    Token m_op;
    std::unique_ptr<Expr> m_right;
};

struct LiteralExpr : public Expr
{
   public:
    LiteralExpr(Token token, int line);

    void accept(ExprVisitor &visitor) override;

    Token token;
};

struct VarExpr : public Expr
{
   public:
    VarExpr(Token token, int scope_depth, int line);

    void accept(ExprVisitor &visitor) override;

    std::unique_ptr<LocalVar> var;
};

struct AssignExpr : public Expr
{
   public:
    AssignExpr(LocalVar var, std::unique_ptr<Expr> expr, int line);

    void accept(ExprVisitor &visitor) override;

    LocalVar var;
    std::unique_ptr<Expr> m_expr;
};

struct UnaryExpr : public Expr
{
   public:
    UnaryExpr(Token op, std::unique_ptr<Expr> right, int line);

    void accept(ExprVisitor &visitor) override;

    Token m_op;
    std::unique_ptr<Expr> m_right;
};

struct GroupingExpr : public Expr
{
   public:
    GroupingExpr(std::unique_ptr<Expr> expr, int line);

    void accept(ExprVisitor &visitor) override;

    std::unique_ptr<Expr> m_expr;
};

#endif
