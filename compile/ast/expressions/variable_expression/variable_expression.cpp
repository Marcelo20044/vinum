#include "variable_expression.h"
#include <stdexcept>

VariableExpression::VariableExpression(std::string name) : name(std::move(name)) {
}

std::shared_ptr<Value> VariableExpression::eval() {
    if (!Variables::isExists(name)) {
        throw std::runtime_error("Constant does not exist: " + name);
    }
    return Variables::get(name);
}
