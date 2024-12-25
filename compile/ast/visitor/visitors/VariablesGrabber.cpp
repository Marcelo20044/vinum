#include "VariablesGrabber.h"
#include "OptimizationVisitor.h"


std::shared_ptr<node>
VariablesGrabber::visitUnaryExpression(UnaryExpression *e, std::unordered_map<std::string, VariableInfo> t) {
    if (auto varExpr = dynamic_cast<VariableExpression *>(e->expr1.get())) {
        grabVariableInfo(t, varExpr->name);
    }

    return OptimizationVisitor::visitUnaryExpression(e, t);
}

std::shared_ptr<node>
VariablesGrabber::visitAssignmentStatement(AssignmentStatement *s, std::unordered_map<std::string, VariableInfo> t) {
    if (s->variable = nullptr)
        return OptimizationVisitor::visitAssignmentStatement(s, t);
}

VariableInfo VariablesGrabber::grabVariableInfo(std::unordered_map<std::string, VariableInfo> t, const std::string& variableName) {
    auto it = t.find(variableName);
    if (it != t.end()) {
        it->second.modifications++;
        return it->second;
    } else {
        VariableInfo var;
        var.modifications = 1;
        auto result = t.emplace(variableName, var);
        return result.first->second;
    }
}

std::unordered_map<std::string, VariableInfo> VariablesGrabber::getInfo(node *node, bool grabModuleConstants) {
    const std::unordered_map<std::string, VariableInfo> variableInfos;
    return node->accept(this, variableInfos)
}