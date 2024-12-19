#include "value_expression.h"
#include "../../../lib/string_value/string_value.h"
#include "../../../lib/double_value/double_value.h"

ValueExpression::ValueExpression(double value)
    : value(std::make_shared<DoubleValue>(value)) {
}

ValueExpression::ValueExpression(const std::string &value)
    : value(std::make_shared<StringValue>(value)) {
}

std::shared_ptr<Value> ValueExpression::eval() {
    return value;
}
