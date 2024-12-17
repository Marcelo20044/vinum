#include "value_expression.h"
#include "../../../lib/string_value/string_value.h"
#include "../../../lib/number_value/number_value.h"

ValueExpression::ValueExpression(double value)
    : value(std::make_shared<NumberValue>(value)) {
}

ValueExpression::ValueExpression(const std::string &value)
    : value(std::make_shared<StringValue>(value)) {
}

std::shared_ptr<Value> ValueExpression::eval() {
    return value;
}
