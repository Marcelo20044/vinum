#ifndef VINUM_CONSTANTPROPAGATION_H
#define VINUM_CONSTANTPROPAGATION_H

#include "../visitor/visitors/OptimizationVisitor.h"
#include "Optimizable.h"

class ConstantPropagation : public OptimizationVisitor<std::unordered_map<std::string, Value>>, public Optimizable {
private:
    std::unordered_map<std::string, int> propagatedVariables;

public:
    ConstantPropagation() : propagatedVariables() {}

    int optimizationsCount() const {
        return static_cast<int>(propagatedVariables.size());
    }

    std::string summaryInfo();

    std::shared_ptr<node> optimize(node *node) override;

    std::shared_ptr<node> visitVariableExpression(VariableExpression *s, std::unordered_map<std::string, Value> t) override;
};


#endif //VINUM_CONSTANTPROPAGATION_H
