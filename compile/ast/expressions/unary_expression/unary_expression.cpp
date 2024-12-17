#include "unary_expression.h"
#include "../../../lib/number_value/number_value.h"

UnaryExpression::UnaryExpression(const char operation, std::shared_ptr<Expression> expr1)
    : expr1(std::move(expr1)), operation(operation) {
}

std::shared_ptr<Value> UnaryExpression::eval() {
    switch (operation) {
        case '-':
            return std::make_shared<NumberValue>(-expr1->eval()->asNumber());
        case '+':
        default:
            return expr1->eval();
    }
}
