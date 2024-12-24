#include "ConstantFolding.h"

std::shared_ptr<node> ConstantFolding::visitBinaryExpression(BinaryExpression *e, std::shared_ptr<node> t) {
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

    return OptimizationVisitor<std::shared_ptr<node>>::visitBinaryExpression(e, t);
}

std::shared_ptr<node> ConstantFolding::visitUnaryExpression(UnaryExpression *e, std::shared_ptr<node> t) {
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

    return OptimizationVisitor<std::shared_ptr<node>>::visitUnaryExpression(e, t);
}

std::shared_ptr<node> ConstantFolding::visitConditionalExpression(ConditionalExpression *e, std::shared_ptr<node> t) {
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

    return OptimizationVisitor<std::shared_ptr<node>>::visitConditionalExpression(e, t);
}

std::shared_ptr<node>
ConstantFolding::visitFunctionDefineStatement(FunctionDefineStatement *s, std::shared_ptr<node> t) {
    return OptimizationVisitor<std::shared_ptr<node>>::visitFunctionDefineStatement(s, t);
}

