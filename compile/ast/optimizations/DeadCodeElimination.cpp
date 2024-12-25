#include "DeadCodeElimination.h"

std::shared_ptr<node>
DeadCodeElimination::visitIfStatement(IfStatement *s, std::unordered_map<std::string, VariableInfo> &t) {
    if (auto varExpr = dynamic_cast<VariableExpression *>(s->expression)) {
        ifStatementEliminatedCount++;
        if (s->expression->eval()->asInt() != 0) {
            return std::static_pointer_cast<node>(s->ifStatement);
        }
        if (s->elseStatement) {
            return std::static_pointer_cast<node>(s->elseStatement);
        }
        return std::make_shared<FunctionStatement>(s->expression);
    }
    return OptimizationVisitor::visitIfStatement(s, t);
}

std::shared_ptr<node>
DeadCodeElimination::visitBlockStatement(BlockStatement *s, std::unordered_map<std::string, VariableInfo> &t) {
    auto result = std::make_shared<BlockStatement>();
    bool changed = false;

    for (const auto &statement: s->statements) {
        auto node = statement->accept(*this, t);
        if (node != statement) {
            changed = true;
        }

        if (auto exprStmt = std::dynamic_pointer_cast<FunctionStatement>(node)) {
            if (isConstantValue(exprStmt->expr)) {
                changed = true;
                continue;
            }
        }

        if (auto stmt = std::dynamic_pointer_cast<Statement>(node)) {
            result->add(stmt);
        } else if (node) {
            result->add(std::make_shared<FunctionStatement>(node));
        }
    }

    if (changed) {
        return std::static_pointer_cast<node>(result);
    }

    return OptimizationVisitor::visitBlockStatement(s, t);
}


int DeadCodeElimination::OptimizationsCount() const {
    return ifStatementEliminatedCount + ternaryExpressionEliminatedCount
           + whileStatementEliminatedCount + assignmentExpressionEliminatedCount;
}

std::string DeadCodeElimination::summaryInfo() const {
    if (OptimizationsCount() == 0) {
        return "";
    }
    std::ostringstream sb;
    if (ifStatementEliminatedCount > 0) {
        sb << "\nEliminated IfStatement: " << ifStatementEliminatedCount;
    }
    if (ternaryExpressionEliminatedCount > 0) {
        sb << "\nEliminated TernaryExpression: " << ternaryExpressionEliminatedCount;
    }
    if (whileStatementEliminatedCount > 0) {
        sb << "\nEliminated WhileStatement: " << whileStatementEliminatedCount;
    }
    if (assignmentExpressionEliminatedCount > 0) {
        sb << "\nEliminated AssignmentExpression: " << assignmentExpressionEliminatedCount;
    }
    return sb.str();
}


