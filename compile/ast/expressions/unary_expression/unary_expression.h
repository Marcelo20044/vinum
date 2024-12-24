#ifndef UNARY_EXPRESSION_H
#define UNARY_EXPRESSION_H

#include "../../ast.h"

class UnaryExpression : public Expression {
public:
    std::shared_ptr<Expression> expr1;
    char operation;

public:
    UnaryExpression(char operation, std::shared_ptr<Expression> expr1);

    std::shared_ptr<Value> eval() override;

    std::shared_ptr<node> accept(visitor *visitor) override {
        return visitor->visitUnaryExpression(this);
    }
};

#endif // UNARY_EXPRESSION_H
