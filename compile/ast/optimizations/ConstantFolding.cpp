#include "ConstantFolding.h"
#include "../../lib/value.h"
#include "ConstantFolding.h"
#include "../expressions/value_expression/value_expression.h"
#include "../expressions/binary_expression/binary_expression.h"
#include "../expressions/value_expression/value_expression.h"
#include "../expressions/unary_expression/unary_expression.h"
#include "../expressions/conditional_expression/conditional_expression.h"
#include "../statements/function_define_statement/function_define_statement.h"


std::shared_ptr<node> ConstantFolding::visitBinaryExpression(BinaryExpression *e, nullptr_t) {
    auto leftValue = std::dynamic_pointer_cast<ValueExpression>(e->expr1);
    auto rightValue = std::dynamic_pointer_cast<ValueExpression>(e->expr2);

    if (leftValue && rightValue) {
        std::shared_ptr<Value> foldedValue = e->eval();

        if (foldedValue->getType() == ValueType::DOUBLE) {
            return std::make_shared<ValueExpression>(foldedValue->asDouble());
        } else if (foldedValue->getType() == ValueType::STRING) {
            return std::make_shared<ValueExpression>(foldedValue->asString());
        } else if (foldedValue->getType() == ValueType::INT) {
            return std::make_shared<ValueExpression>(foldedValue->asInt());
        }
    }

    return OptimizationVisitor<std::shared_ptr<node>>::visitBinaryExpression(e);
}

std::shared_ptr<node> ConstantFolding::visitUnaryExpression(UnaryExpression *e, nullptr_t) {
    std::shared_ptr<Expression> exprValue = std::dynamic_pointer_cast<ValueExpression>(e->expr1);

    if (exprValue) {
        std::shared_ptr<Value> foldedValue = e->eval();

        if (foldedValue->getType() == ValueType::DOUBLE) {
            return std::make_shared<ValueExpression>(foldedValue->asDouble());
        } else if (foldedValue->getType() == ValueType::STRING) {
            return std::make_shared<ValueExpression>(foldedValue->asString());
        } else if (foldedValue->getType() == ValueType::INT) {
            return std::make_shared<ValueExpression>(foldedValue->asInt());
        }
    }

    return OptimizationVisitor<std::shared_ptr<node>>::visitUnaryExpression(e, nullptr_t);
}

std::shared_ptr<node> ConstantFolding::visitConditionalExpression(ConditionalExpression *e, nullptr_t) {
    auto leftValue = std::dynamic_pointer_cast<ValueExpression>(e->expr1);
    auto rightValue = std::dynamic_pointer_cast<ValueExpression>(e->expr2);

    if (leftValue && rightValue) {
        std::shared_ptr<Value> foldedValue = e->eval();

        if (foldedValue->getType() == ValueType::DOUBLE) {
            return std::make_shared<ValueExpression>(foldedValue->asDouble());
        } else if (foldedValue->getType() == ValueType::STRING) {
            return std::make_shared<ValueExpression>(foldedValue->asString());
        } else if (foldedValue->getType() == ValueType::INT) {
            return std::make_shared<ValueExpression>(foldedValue->asInt());
        }
    }

    return OptimizationVisitor<std::shared_ptr<node>>::visitConditionalExpression(e, nullptr_t);
}

std::shared_ptr<node> ConstantFolding::visitFunctionDefineStatement(FunctionDefineStatement *s, nullptr_t) {
    return OptimizationVisitor<std::shared_ptr<node>>::visitFunctionDefineStatement(s, nullptr_t);
}

