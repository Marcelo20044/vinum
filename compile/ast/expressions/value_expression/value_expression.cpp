#include "value_expression.h"

#include <utility>
#include "../../../lib/string_value/string_value.h"
#include "../../../lib/double_value/double_value.h"
#include "../../../lib/int_value/int_value.h"
#include "../../../lib/long_value/long_value.h"

ValueExpression::ValueExpression(double value)
    : value(std::make_shared<DoubleValue>(value)) {
}

ValueExpression::ValueExpression(const std::string &value)
    : value(std::make_shared<StringValue>(value)) {
}

ValueExpression::ValueExpression(int value)
    : value(std::make_shared<IntValue>(value)) {
}

ValueExpression::ValueExpression(long long value)
    : value(std::make_shared<LongValue>(value)) {
}

ValueExpression::ValueExpression(std::shared_ptr<Value> value) : value(std::move(value)) {
}

std::shared_ptr<Value> ValueExpression::eval() {
    return value;
}

std::shared_ptr<Expression> ValueExpression::getValExpression(const std::shared_ptr<Value>& value) {
    return std::make_shared<ValueExpression>(value);
}
