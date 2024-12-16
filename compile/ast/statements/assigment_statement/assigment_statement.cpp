#include "assigment_statement.h"

AssignmentStatement::AssignmentStatement(const std::string& variable, Expression* expression)
    : variable(variable), expression(expression) {}

void AssignmentStatement::execute() {
    std::shared_ptr<Value> result = expression->eval();
    Variables::set(variable, result);
}

AssignmentStatement::~AssignmentStatement() {
    delete expression;
}