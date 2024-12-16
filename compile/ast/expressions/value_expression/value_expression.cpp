#include "value_expression.h"

ValueExpression::ValueExpression(double value)
    : value(std::make_shared<NumberValue>(value)) {}

ValueExpression::ValueExpression(const std::string& value)
    : value(std::make_shared<StringValue>(value)) {}

std::shared_ptr<Value> ValueExpression::eval() {
    return value;
}
