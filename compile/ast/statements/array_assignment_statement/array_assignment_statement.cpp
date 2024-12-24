#include "array_assignment_statement.h"
#include "../../../lib/array_value/array_value.h"

class ArrayValue;

ArrayAssignmentStatement::ArrayAssignmentStatement(std::string  variable,
                                                   const std::shared_ptr<Expression>& index,
                                                   const std::shared_ptr<Expression>& expression)
    : variable(std::move(variable)), index(index), expression(expression) {}

void ArrayAssignmentStatement::execute() {
    auto var = Variables::get(variable);
    auto array = std::dynamic_pointer_cast<ArrayValue>(var);
    if (!array) {
        throw std::runtime_error("Array expected");
    }
    size_t idx = static_cast<size_t>(index->eval()->asInt());
    array->set(idx, expression->eval());
}