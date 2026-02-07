#include "expr.h"

void LiteralExpr::accept(ExprVisitor& visitor) const {
    visitor.visitLiteral(*this);
}

void VariableExpr::accept(ExprVisitor& visitor) const {
    visitor.visitVariable(*this);
}

void BinaryExpr::accept(ExprVisitor& visitor) const {
    visitor.visitBinary(*this);
}

void UnaryExpr::accept(ExprVisitor& visitor) const {
    visitor.visitUnary(*this);
}

void GroupingExpr::accept(ExprVisitor& visitor) const {
    visitor.visitGrouping(*this);
}

void IfExpr::accept(ExprVisitor& visitor) const {
    visitor.visitIf(*this);
}

void SwitchExpr::accept(ExprVisitor& visitor) const {
    visitor.visitSwitch(*this);
}

void FunctionExpr::accept(ExprVisitor& visitor) const {
    visitor.visitFunction(*this);
}
