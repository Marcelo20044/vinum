#include "conditional_expression.h"

#include "../../../lib/boolean_value/boolean_value.h"
#include "../../../lib/string_value/string_value.h"
#include "../../../lib/boolean_value//boolean_value.h"
#include "../value_expression/value_expression.h"

ConditionalExpression::ConditionalExpression(const Operator operation, std::shared_ptr<Expression> expr1,
                                             std::shared_ptr<Expression> expr2)
    : expr1(std::move(expr1)), expr2(std::move(expr2)), operation(operation) {
}

std::shared_ptr<Value> ConditionalExpression::eval() {
    const std::shared_ptr<Value> value1 = expr1->eval();
    const std::shared_ptr<Value> value2 = expr2->eval();

    double number1, number2;

    if (dynamic_cast<StringValue *>(value1.get())) {
        number1 = value1->asString().compare(value2->asString());
        number2 = 0;
    } else {
        number1 = value1->asDouble();
        number2 = value2->asDouble();
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

    return std::make_shared<BooleanValue>(result);
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

std::shared_ptr<Expression> ConditionalExpression::optimize() {
    const std::shared_ptr<Expression> left = expr1->optimize();
    const std::shared_ptr<Expression> right = expr2->optimize();

    std::shared_ptr<Expression> leftValue = std::dynamic_pointer_cast<ValueExpression>(left);
    std::shared_ptr<Expression> rightValue = std::dynamic_pointer_cast<ValueExpression>(right);

    // Constant Folding
    if (leftValue && rightValue) {
        std::shared_ptr<Value> foldedValue = this->eval();

        if (foldedValue->getType() == ValueType::DOUBLE) {
            return std::make_shared<ValueExpression>(foldedValue->asDouble());
        } else if (foldedValue->getType() == ValueType::STRING) {
            return std::make_shared<ValueExpression>(foldedValue->asString());
        } else if (foldedValue->getType() == ValueType::INT) {
            return std::make_shared<ValueExpression>(foldedValue->asInt());
        }
    }

    // Dead Code Elimination
    if (operation == Operator::AND) {
        // x && true -> x, x && false -> false
        if (leftValue) {
            if (!leftValue->eval()->asBoolean()) {
                return std::make_shared<ValueExpression>(false);
            }
        }
        if (rightValue) {
            if (!rightValue->eval()->asBoolean()) {
                return std::make_shared<ValueExpression>(false);
            }
        }
    } else if (operation == Operator::OR) {
        //  x || true -> true, x || false -> x
        if (leftValue) {
            if (leftValue->eval()->asBoolean()) {
                return std::make_shared<ValueExpression>(true);
            }
        }
        if (rightValue) {
            if (rightValue->eval()->asBoolean()) {
                return std::make_shared<ValueExpression>(true);
            }
        }
    }

    return std::make_shared<ConditionalExpression>(operation, left, right);
}