#include "return_statement.h"

ReturnStatement::ReturnStatement(std::shared_ptr<Expression> expression)
    : std::runtime_error("Return statement"), expression(std::move(expression)), result(nullptr) {
}

std::shared_ptr<Value> ReturnStatement::getResult() const {
    return result;
}

void ReturnStatement::execute() {
    result = expression->eval();
    throw *this;
}
