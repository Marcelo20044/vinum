#include "unary_expression.h"

UnaryExpression::UnaryExpression(char operation, std::shared_ptr<Expression> expr1)
    : operation(operation), expr1(std::move(expr1)) {}

std::shared_ptr<Value> UnaryExpression::eval() {
    switch (operation) {
        case '-':
            return std::make_shared<NumberValue>(-expr1->eval()->asNumber());
        case '+':
            default:
                return expr1->eval();
    }
}
