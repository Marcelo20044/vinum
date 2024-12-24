#ifndef VINUM_CONSTANTPROPAGATION_H
#define VINUM_CONSTANTPROPAGATION_H

#include "../visitor/visitors/OptimizationVisitor.h"

class ConstantPropagation : public OptimizationVisitor<std::shared_ptr<node>> {
private:
    std::unordered_map<std::string, int> propagatedVariables;

public:
    ConstantPropagation() : propagatedVariables() {}

    int optimizationsCount() const {
        return static_cast<int>(propagatedVariables.size());
    }

    std::shared_ptr<node> optimize(node *node);

    std::shared_ptr<node> visitVariableExpression(VariableExpression *s, std::shared_ptr<node> t) override;
};


#endif //VINUM_CONSTANTPROPAGATION_H
