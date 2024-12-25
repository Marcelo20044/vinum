#ifndef VINUM_CONSTANTFOLDING_H
#define VINUM_CONSTANTFOLDING_H

#include "../visitor/visitors/OptimizationVisitor.h"
#include "Optimizable.h"

class ConstantFolding : public OptimizationVisitor<nullptr_t>, public Optimizable {
public:

    std::shared_ptr<node> optimize(node *node) override {
        return node->accept(*this);
    }

    std::shared_ptr<node> visitBinaryExpression(BinaryExpression *expression, nullptr_t) override;

    std::shared_ptr<node> visitConditionalExpression(ConditionalExpression *expression, nullptr_t) override;

    std::shared_ptr<node> visitUnaryExpression(UnaryExpression *expression, nullptr_t) override;

    std::shared_ptr<node> visitFunctionDefineStatement(FunctionDefineStatement *statement, nullptr_t) override;
};


#endif //VINUM_CONSTANTFOLDING_H
