#pragma once
#include <memory>
#include <vector>
#include "token.h"

struct ExprVisitor {
    virtual void visitLiteral(const LiteralExpr&) = 0;
    virtual void visitVariable(const VariableExpr&) = 0;
    virtual void visitBinary(const BinaryExpr&) = 0;
    virtual void visitUnary(const UnaryExpr&) = 0;
    virtual void visitGrouping(const GroupingExpr&) = 0;
    virtual void visitIf(const IfExpr&) = 0;
    virtual void visitSwitch(const SwitchExpr&) = 0;
    virtual void visitFunction(const FunctionExpr&) = 0;
};

struct Expr {
    virtual ~Expr() = default;
    virtual void accept(ExprVisitor& visitor) const = 0;
};

using ExprPtr = std::unique_ptr<Expr>;

struct FunctionExpr : Expr {
    std::vector<Token> params;
    ExprPtr body;

    FunctionExpr(std::vector<Token> p, ExprPtr b) : params(std::move(p)), body(std::move(b)) {}

    void accept(ExprVisitor& visitor) const override;
};

struct GroupingExpr : Expr {
    ExprPtr expression;

    explicit GroupingExpr(ExprPtr expr) : expression(std::move(expr)) {}

    void accept(ExprVisitor& visitor) const override;
};

struct BinaryExpr : Expr {
    ExprPtr left;
    Token op;
    ExprPtr right;

    BinaryExpr(ExprPtr l, Token o, ExprPtr r)
        : left(std::move(l)), op(o), right(std::move(r)) {}

    void accept(ExprVisitor& visitor) const override;
};

struct IfExpr : Expr {
    ExprPtr condition;
    ExprPtr thenBranch;
    ExprPtr elseBranch;

    IfExpr(ExprPtr cond, ExprPtr thenB, ExprPtr elseB)
        : condition(std::move(cond)), thenBranch(std::move(thenB)), elseBranch(std::move(elseB)) {}

    void accept(ExprVisitor& visitor) const override;
};

struct LiteralExpr : Expr {
    Token value;

    explicit LiteralExpr(Token val) : value(val) {}

    void accept(ExprVisitor& visitor) const override;
};

struct SwitchCase {
    ExprPtr pattern;
    ExprPtr body;

    SwitchCase(ExprPtr p, ExprPtr b) : pattern(std::move(p)), body(std::move(b)) {}
};

struct SwitchExpr : Expr {
    ExprPtr value;
    std::vector<SwitchCase> cases;
    ExprPtr defaultCase;

    SwitchExpr(ExprPtr val, std::vector<SwitchCase> cs, ExprPtr def)
        : value(std::move(val)), cases(std::move(cs)), defaultCase(std::move(def)) {}

    void accept(ExprVisitor& visitor) const override;
};

struct UnaryExpr : Expr {
    Token op;
    ExprPtr right;

    UnaryExpr(Token o, ExprPtr r) : op(o), right(std::move(r)) {}

    void accept(ExprVisitor& visitor) const override;
};

struct VariableExpr : Expr {
    Token name;

    explicit VariableExpr(Token n) : name(n) {}

    void accept(ExprVisitor& visitor) const override;
};
