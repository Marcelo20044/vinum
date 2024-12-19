#include "initialization_statement.h"

InitializationStatement::InitializationStatement(ValueType varType, std::string variable,
    const std::shared_ptr<Expression> &expression) : varType(varType), varName(std::move(variable)), expression(expression) {
}

void InitializationStatement::execute() {
    if (Variables::isExists(varName)) {
        throw std::runtime_error("Variable '" + varName + "' already exists");
    }
    std::shared_ptr<Value> result = Value::asType(expression->eval(), varType);
    Variables::set(varName, result);
}
