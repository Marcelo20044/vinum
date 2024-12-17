#ifndef BINARY_EXPRESSION_H
#define BINARY_EXPRESSION_H

#include "../../ast.h"

class BinaryExpression : public Expression {
    std::shared_ptr<Expression> expr1;
    std::shared_ptr<Expression> expr2;
    char operation;

public:
    BinaryExpression(char operation, std::shared_ptr<Expression> expr1, std::shared_ptr<Expression> expr2);

    std::shared_ptr<Value> eval() override;
};

#endif // BINARY_EXPRESSION_H
