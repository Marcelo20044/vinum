#ifndef VALUE_EXPRESSION_H
#define VALUE_EXPRESSION_H

#include <memory>
#include "../../ast.h"

class ValueExpression : public Expression {
    std::shared_ptr<Value> value;

public:
    explicit ValueExpression(double value);

    explicit ValueExpression(const std::string &value);

    std::shared_ptr<Value> eval() override;
};

#endif // VALUE_EXPRESSION_H
