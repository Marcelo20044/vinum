//#ifndef VINUM_DEADCODEELIMINATION_H
//#define VINUM_DEADCODEELIMINATION_H
//
//#include "../visitor/visitors/OptimizationVisitor.h"
//#include "Optimizable.h"
//#include "../visitor/visitors/VariablesGrabber.h"
//#include "../visitor/node.h"
//
//class DeadCodeElimination
//        : public OptimizationVisitor<std::unordered_map<std::string, VariableInfo>>, public Optimizable {
//private:
//    int ifStatementEliminatedCount = 0;
//    int ternaryExpressionEliminatedCount = 0;
//    int whileStatementEliminatedCount = 0;
//    int assignmentExpressionEliminatedCount = 0;
//public:
//    std::shared_ptr<node> optimize(node *node) override;
//
//    int OptimizationsCount() const;
//
//    std::string summaryInfo() const;
//
//    std::shared_ptr<node> visitIfStatement(IfStatement *s, std::unordered_map<std::string, VariableInfo> t) override;
//
//    std::shared_ptr<node> visitBlockStatement(BlockStatement *s,  std::unordered_map<std::string, VariableInfo> t) override;
//};
//
//
//#endif //VINUM_DEADCODEELIMINATION_H