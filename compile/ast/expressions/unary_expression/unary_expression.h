#ifndef UNARY_EXPRESSION_H
#define UNARY_EXPRESSION_H

#include "../../ast.h"
#include <memory>

class UnaryExpression : public Expression {
private:
    std::shared_ptr<Expression> expr1;
    char operation;

public:
    UnaryExpression(char operation, std::shared_ptr<Expression> expr1);

    std::shared_ptr<Value> eval() override;
};

#endif // UNARY_EXPRESSION_H
