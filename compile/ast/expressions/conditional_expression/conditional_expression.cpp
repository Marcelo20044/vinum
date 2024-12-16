#include "conditional_expression.h"

ConditionalExpression::ConditionalExpression(Operator operation, std::shared_ptr<Expression> expr1, std::shared_ptr<Expression> expr2)
    : operation(operation), expr1(std::move(expr1)), expr2(std::move(expr2)) {}

std::shared_ptr<Value> ConditionalExpression::eval() {
    std::shared_ptr<Value> value1 = expr1->eval();
    std::shared_ptr<Value> value2 = expr2->eval();

    double number1, number2;

    if (dynamic_cast<StringValue*>(value1.get())) {
        number1 = value1->asString().compare(value2->asString());
        number2 = 0;
    } else {
        number1 = value1->asNumber();
        number2 = value2->asNumber();
    }

    bool result;
    switch (operation) {
        case Operator::LT:
            result = number1 < number2;
            break;
        case Operator::LTEQ:
            result = number1 <= number2;
            break;
        case Operator::GT:
            result = number1 > number2;
            break;
        case Operator::GTEQ:
            result = number1 >= number2;
            break;
        case Operator::NOT_EQUALS:
            result = number1 != number2;
            break;
        case Operator::AND:
            result = (number1 != 0) && (number2 != 0);
            break;
        case Operator::OR:
            result = (number1 != 0) || (number2 != 0);
            break;
        case Operator::EQUALS:
        default:
            result = number1 == number2;
            break;
    }

    return std::make_shared<NumberValue>(result);
}

std::string ConditionalExpression::operatorToString(Operator op) {
    switch (op) {
        case Operator::EQUALS:
            return "==";
        case Operator::NOT_EQUALS:
            return "!=";
        case Operator::LT:
            return "<";
        case Operator::LTEQ:
            return "<=";
        case Operator::GT:
            return ">";
        case Operator::GTEQ:
            return ">=";
        case Operator::AND:
            return "&&";
        case Operator::OR:
            return "||";
        default:
            return "unknown";
    }
}
