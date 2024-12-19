#include "assigment_statement.h"

AssignmentStatement::AssignmentStatement(std::string variable, const std::shared_ptr<Expression> &expression)
    : variable(std::move(variable)), expression(expression) {
}

void AssignmentStatement::execute() {
    std::shared_ptr<Value> oldVal = Variables::get(variable);
    std::shared_ptr<Value> result = Value::asType(expression->eval(), oldVal->getType());
    Variables::set(variable, result);
}
