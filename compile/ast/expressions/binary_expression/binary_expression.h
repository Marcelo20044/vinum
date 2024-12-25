#ifndef BINARY_EXPRESSION_H
#define BINARY_EXPRESSION_H

#include "../../ast.h"

class BinaryExpression : public Expression {
public:
    std::shared_ptr<Expression> expr1;
    std::shared_ptr<Expression> expr2;
    char operation;

public:
    BinaryExpression(char operation, std::shared_ptr<Expression> expr1, std::shared_ptr<Expression> expr2);

    std::shared_ptr<Value> eval() override;

    std::shared_ptr<node> accept(visitor *visitor) override {
        return visitor->visitBinaryExpression(this);
    }
};

#endif // BINARY_EXPRESSION_H
