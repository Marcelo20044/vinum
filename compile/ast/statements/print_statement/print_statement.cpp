#include "print_statement.h"

#include <iostream>

PrintStatement::PrintStatement(std::shared_ptr<Expression> expression)
    : expression(std::move(expression)) {
}

void PrintStatement::execute() {
    std::cout << expression->eval()->asString();
}
