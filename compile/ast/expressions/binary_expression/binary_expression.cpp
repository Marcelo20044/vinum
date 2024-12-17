#include "binary_expression.h"

#include <sstream>

#include "../../../lib/string_value/string_value.h"
#include "../../../lib/number_value/number_value.h"
#include <stdexcept>

BinaryExpression::BinaryExpression(char operation, std::shared_ptr<Expression> expr1, std::shared_ptr<Expression> expr2)
    : expr1(std::move(expr1)), expr2(std::move(expr2)), operation(operation) {
}

std::shared_ptr<Value> BinaryExpression::eval() {
    const std::shared_ptr<Value> value1 = expr1->eval();
    const std::shared_ptr<Value> value2 = expr2->eval();

    if (dynamic_cast<StringValue *>(value1.get())) {
        const std::string string1 = value1->asString();
        switch (operation) {
            case '*': {
                const int iterations = static_cast<int>(value2->asNumber());
                std::ostringstream buffer;
                for (int i = 0; i < iterations; ++i) {
                    buffer << string1;
                }
                return std::make_shared<StringValue>(buffer.str());
            }
            case '+':
            default:
                return std::make_shared<StringValue>(string1 + value2->asString());
        }
    }

    const double number1 = value1->asNumber();
    const double number2 = value2->asNumber();
    switch (operation) {
        case '-':
            return std::make_shared<NumberValue>(number1 - number2);
        case '*':
            return std::make_shared<NumberValue>(number1 * number2);
        case '/':
            if (number2 == 0) {
                throw std::runtime_error("Division by zero");
            }
            return std::make_shared<NumberValue>(number1 / number2);
        case '+':
        default:
            return std::make_shared<NumberValue>(number1 + number2);
    }
}
