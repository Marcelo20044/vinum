#include "array_access_expression.h"
#include "../../../lib/array_value/array_value.h"

ArrayAccessExpression::ArrayAccessExpression(std::string variable, const std::shared_ptr<Expression> &index)
    : variable(std::move(variable)),
      index(index) {
}

std::shared_ptr<Value> ArrayAccessExpression::eval() {
    auto var = Variables::get(variable);
    auto array = std::dynamic_pointer_cast<ArrayValue>(var);
    if (!array) {
        throw std::runtime_error("Array expected");
    }
    size_t idx = static_cast<size_t>(index->eval()->asInt());
    return array->get(idx);
}
