//#include "ConstantPropagation.h"
//#include "VariableInfo.h"
//#include "../visitor/visitors/VariablesGrabber.h"
//#include "../expressions/variable_expression/variable_expression.h"
//
//std::shared_ptr<node> ConstantPropagation::optimize(node *node) {
//    const std::unordered_map<std::string, VariableInfo> variables;
//
//    VariablesGrabber grabber(true);
//    node->acceptResultVisitor(grabber, variables);
//
//    std::unordered_map<std::string, Value> candidates;
//    for (const auto &entry: variables) {
//        const VariableInfo &info = entry.second;
//        if (info.modifications != 1) continue;
//        if (!info.value) continue;
//
//        switch (info.value->getType()) {
//            case ValueType::SHORT:
//            case ValueType::INT:
//            case ValueType::LONG:
//            case ValueType::DOUBLE:
//            case ValueType::STRING:
//                candidates[entry.first] = *info.value;
//                break;
//            default:
//                break;
//        }
//    }
//
//    return node->acceptResultVisitor(*this, candidates);
//}
//
//std::shared_ptr<node> ConstantPropagation::visitVariableExpression(VariableExpression *s, std::unordered_map<std::string, Value> t) {
//    auto it = t.find(s->name);
//    if (it != t.end()) {
//        const std::string &varName = s->name;
//        if (propagatedVariables.find(varName) == propagatedVariables.end()) {
//            propagatedVariables[varName] = 1;
//        } else {
//            propagatedVariables[varName]++;
//        }
//        return std::make_shared<ValueExpression>(it->second);
//    }
//
//    return OptimizationVisitor::visitVariableExpression(s, t);
//}
//
//std::string ConstantPropagation::summaryInfo() {
//    if (optimizationsCount() == 0) {
//        return "";
//    }
//
//    std::ostringstream sb;
//
//    if (!propagatedVariables.empty()) {
//        sb << "\nConstant propagations: " << propagatedVariables.size();
//        for (const auto &e: propagatedVariables) {
//            sb << "\n  " << e.first << ": " << e.second;
//        }
//    }
//
//    return sb.str();
//}