#ifndef VINUM_CONSTANTFOLDING_H
#define VINUM_CONSTANTFOLDING_H

#include "../visitor/visitors/OptimizationVisitor.h"

class ConstantFolding : public OptimizationVisitor<std::shared_ptr<node>> {
public:

    // должен быть тип void в OptimizationVisitor но он не хочет
    std::shared_ptr<node> optimize(node *node) {
        return node->accept(this, nullptr);
    }

    std::shared_ptr<node> visitBinaryExpression(BinaryExpression *expression, std::shared_ptr<node> t) override;

    std::shared_ptr<node> visitConditionalExpression(ConditionalExpression *expression, std::shared_ptr<node> t) override;

    std::shared_ptr<node> visitUnaryExpression(UnaryExpression *expression, std::shared_ptr<node> t) override;

    std::shared_ptr<node> visitFunctionDefineStatement(FunctionDefineStatement *statement, std::shared_ptr<node> t) override;
};


#endif //VINUM_CONSTANTFOLDING_H
