#ifndef VALUE_EXPRESSION_H
#define VALUE_EXPRESSION_H

#include <memory>
#include "../../ast.h"

class ValueExpression : public Expression {

public:
    explicit ValueExpression(double value);

    explicit ValueExpression(const std::string &value);

    explicit ValueExpression(int value);

    std::shared_ptr<Value> eval() override;

    std::shared_ptr<Value> value;
};

#endif // VALUE_EXPRESSION_H
