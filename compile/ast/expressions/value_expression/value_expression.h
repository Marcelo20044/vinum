#ifndef VALUE_EXPRESSION_H
#define VALUE_EXPRESSION_H

#include "../../ast.h"
#include <memory>

class ValueExpression : public Expression {
private:
    std::shared_ptr<Value> value;

public:
    explicit ValueExpression(double value);
    explicit ValueExpression(const std::string& value);

    std::shared_ptr<Value> eval() override;
};

#endif // VALUE_EXPRESSION_H
