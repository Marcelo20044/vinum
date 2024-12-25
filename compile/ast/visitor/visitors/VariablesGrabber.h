#ifndef VINUM_VARIABLESGRABBER_H
#define VINUM_VARIABLESGRABBER_H

#include "../../optimizations/VariableInfo.h"
#include "OptimizationVisitor.h"
#include "../../visitor/node.h"

class VariablesGrabber : public OptimizationVisitor<std::unordered_map<std::string, VariableInfo>> {
private:
    bool grabModuleConstants;
public:
    VariablesGrabber() {
        this->grabModuleConstants = false;
    }

    explicit VariablesGrabber(bool grabModuleConstants) {
        this->grabModuleConstants = grabModuleConstants;
    };

    static std::unordered_map<std::string, VariableInfo> getInfo(node *node, bool grabModuleConstants);

    VariableInfo grabVariableInfo(std::unordered_map<std::string, VariableInfo> t, const std::string &variableName);

    std::shared_ptr<node> visitAssignmentStatement(AssignmentStatement *statement, std::unordered_map<std::string, VariableInfo> t);

    std::shared_ptr<node> visitUnaryExpression(UnaryExpression *expression, std::unordered_map<std::string, VariableInfo> t);
};


#endif //VINUM_VARIABLESGRABBER_H
