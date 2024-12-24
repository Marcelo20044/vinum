#include "assigment_statement.h"
#include "../../../lib/array_value/array_value.h"

AssignmentStatement::AssignmentStatement(std::string variable, const std::shared_ptr<Expression> &expression)
    : variable(std::move(variable)), expression(expression) {
}

void AssignmentStatement::execute() {
    expression = expression->optimize();

    std::shared_ptr<Value> oldVal = Variables::get(variable);
    std::shared_ptr<Value> result = Value::asType(expression->eval(), oldVal->getType());

    if (result->getType() == ValueType::ARRAY && oldVal->getType() != ValueType::ARRAY) {
        auto array1 = std::dynamic_pointer_cast<ArrayValue>(result);
        auto array2 = std::dynamic_pointer_cast<ArrayValue>(oldVal);
        if (array1->getElemsType() != array2->getElemsType()) {
            throw std::runtime_error("Cannot assign []" + Value::getTypeString(array2->getElemsType()) + " to []" + Value::getTypeString(array1->getElemsType()));
        }
        Variables::set(variable, array2);
    }

    Variables::set(variable, Value::asType(result, oldVal->getType()));
}
