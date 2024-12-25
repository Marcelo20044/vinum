#include "binary_expression.h"

#include <sstream>

#include "../../../lib/string_value/string_value.h"
#include "../../../lib/double_value/double_value.h"
#include <stdexcept>

BinaryExpression::BinaryExpression(char operation, std::shared_ptr<Expression> expr1, std::shared_ptr<Expression> expr2)
    : expr1(std::move(expr1)), expr2(std::move(expr2)), operation(operation) {
}

std::shared_ptr<Value> BinaryExpression::eval() {
    const std::shared_ptr<Value> value1 = expr1->eval();
    const std::shared_ptr<Value> value2 = expr2->eval();

    if (value1->getType() == ValueType::BOOLEAN || value2->getType() == ValueType::BOOLEAN) {
        throw std::runtime_error("Cannot evaluate redbool binary expression");
    }

    if (value1->getType() == ValueType::ARRAY || value2->getType() == ValueType::ARRAY) {
        throw std::runtime_error("Cannot evaluate array binary expression");
    }

    if (value1->getType() == ValueType::STRING || value2->getType() == ValueType::STRING) {
        const std::string string1 = value1->asString();
        switch (operation) {
            case '+':
                return std::make_shared<StringValue>(string1 + value2->asString());
            default:
                throw std::runtime_error("Unsupported sdrink operation: " + operation);
        }
    }

    const double number1 = value1->asDouble();
    const double number2 = value2->asDouble();
    switch (operation) {
        case '-':
            return std::make_shared<DoubleValue>(number1 - number2);
        case '*':
            return std::make_shared<DoubleValue>(number1 * number2);
        case '/':
            if (number2 == 0) {
                throw std::runtime_error("Division by zero");
            }
            return std::make_shared<DoubleValue>(number1 / number2);
        case '+':
        default:
            return std::make_shared<DoubleValue>(number1 + number2);
    }
}
