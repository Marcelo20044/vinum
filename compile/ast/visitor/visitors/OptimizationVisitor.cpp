#include "OptimizationVisitor.h"
#include "../../statements/assigment_statement/assigment_statement.h"
#include "../../statements/block_statement/block_statement.h"
#include "../../statements/break_statement/break_statement.h"
#include "../../statements/for_statement/for_statement.h"
#include "../../statements/function_define_statement/function_define_statement.h"
#include "../../statements/function_statement/function_statement.h"
#include "../../statements/if_statement/if_statement.h"
#include "../../statements/array_assignment_statement/array_assignment_statement.h"
#include "../../statements/array_initialization_statement/array_initialization_statement.h"
#include "../../statements/print_statement/print_statement.h"
#include "../../statements/return_statement/return_statement.h"
#include "../../expressions/binary_expression/binary_expression.h"
#include "../../expressions/array_access_expression/array_access_expression.h"
#include "../../expressions/conditional_expression/conditional_expression.h"
#include "../../expressions/unary_expression/unary_expression.h"
#include "../../expressions/value_expression/value_expression.h"
#include "../../expressions/variable_expression/variable_expression.h"

template<typename T>
std::shared_ptr<node>
OptimizationVisitor<T>::OptimizationVisitor<T>::visitAssignmentStatement(AssignmentStatement *statement, T t) {
    auto variable = statement->variable;
    auto optimizedExpr = statement->expression->acceptResultVisitor(*this, t);

    if (optimizedExpr != statement->expression) {
        return std::make_shared<AssignmentStatement>(variable, optimizedExpr);
    }

    return std::make_shared<AssignmentStatement>(statement);
}

template<typename T>
std::shared_ptr<node>
OptimizationVisitor<T>::OptimizationVisitor<T>::visitBlockStatement(BlockStatement *statement, T t) {
    auto result = std::make_shared<BlockStatement>();
    bool changed = false;

    for (auto &stmt: statement->statements) {
        auto optimized = stmt->acceptResultVisitor(*this, t);
        if (optimized != stmt) {
            changed = true;
        }
        if (optimized) {
            result->add(optimized);
        }
    }

    return changed ? result : std::make_shared<BlockStatement>(statement, t);
}

template<typename T>
std::shared_ptr<node> OptimizationVisitor<T>::visitBreakStatement(BreakStatement *statement, T t) {
    return std::make_shared<BreakStatement>(statement, t);
}

template<typename T>
std::shared_ptr<node> OptimizationVisitor<T>::visitContinueStatement(ContinueStatement *statement, T t) {
    return std::make_shared<ContinueStatement>(statement, t);
}

template<typename T>
std::shared_ptr<node> OptimizationVisitor<T>::visitForStatement(ForStatement *statement, T t) {
    auto init = statement->initialization->acceptResultVisitor(*this, t);
    auto term = statement->termination->acceptResultVisitor(*this, t);
    auto incr = statement->increment->acceptResultVisitor(*this, t);
    auto stmt = statement->statement->acceptResultVisitor(*this, t);

    if (init != statement->initialization || term != statement->termination || incr != statement->increment ||
        stmt != statement->statement) {
        return std::make_shared<ForStatement>(init, term, incr, stmt);
    }

    return std::make_shared<ForStatement>(statement, t);
}

template<typename T>
std::shared_ptr<node> OptimizationVisitor<T>::visitFunctionDefineStatement(FunctionDefineStatement *statement, T t) {
    auto body = statement->body->acceptResultVisitor(*this, t);
    if (body != statement->body) {
        return std::make_shared<FunctionDefineStatement>(statement->name, statement->returnType, statement->args, body);
    }
    return std::make_shared<FunctionDefineStatement>(statement, t);
}

template<typename T>
std::shared_ptr<node> OptimizationVisitor<T>::visitIfStatement(IfStatement *statement, T t) {
    auto expr = statement->expression->acceptResultVisitor(*this, t);
    auto ifStmt = statement->ifStatement->acceptResultVisitor(*this, t);
    auto elseStmt = statement->elseStatement ? statement->elseStatement->acceptResultVisitor(*this, t) : nullptr;

    if (expr != statement->expression || ifStmt != statement->ifStatement || elseStmt != statement->elseStatement) {
        return std::make_shared<IfStatement>(expr, ifStmt, elseStmt);
    }

    return std::make_shared<IfStatement>(statement, t);
}

template<typename T>
std::shared_ptr<node> OptimizationVisitor<T>::visitBinaryExpression(BinaryExpression *expression, T t) {
    auto optimizedLeft = expression->expr1->acceptResultVisitor(*this, t);
    auto optimizedRight = expression->expr2->acceptResultVisitor(*this, t);

    if (optimizedLeft != expression->expr1 || optimizedRight != expression->expr2) {
        return std::make_shared<BinaryExpression>(optimizedLeft, optimizedRight);
    }

    return std::make_shared<BinaryExpression>(expression, t);
}

template<typename T>
std::shared_ptr<node> OptimizationVisitor<T>::visitFunctionalExpression(FunctionalExpression *expression, T t) {
    auto result = std::make_shared<FunctionalExpression>(expression);
    bool changed = false;

    for ( const auto &argument : result->arguments) {
        auto expr = argument->acceptResultVisitor(*this, t);
        if (expr != argument) {
            changed = true;
        }
        result->addArgument(expr);
    }

    return changed ? result : std::make_shared<FunctionalExpression>(expression, t);
}

template<typename T>
std::shared_ptr<node> OptimizationVisitor<T>::visitReturnStatement(ReturnStatement *statement, T t) {
    auto expression = statement->expression->acceptResultVisitor(*this, t);
    if (expression != statement->expression) {
        return std::make_shared<ReturnStatement>(expression);
    }
    return std::make_shared<ReturnStatement>(statement, t);
}

template<typename T>
std::shared_ptr<node> OptimizationVisitor<T>::visitPrintStatement(PrintStatement *statement, T t) {
    auto expression = statement->expression->acceptResultVisitor(*this, t);
    if (expression != statement->expression) {
        return std::make_shared<PrintStatement>(expression);
    }
    return std::make_shared<PrintStatement>(statement, t);
}

template<typename T>
std::shared_ptr<node> OptimizationVisitor<T>::visitConditionalExpression(ConditionalExpression *expression, T t) {
    auto expr1 = expression->expr1->acceptResultVisitor(*this, t);
    auto expr2 = expression->expr2->acceptResultVisitor(*this, t);

    if (expr1 != expression->expr1 || expr2 != expression->expr2) {
        return std::make_shared<ConditionalExpression>(expression->operation, expr1, expr2);
    }

    return std::make_shared<ConditionalExpression>(expression, t);
}

template<typename T>
std::shared_ptr<node> OptimizationVisitor<T>::visitUnaryExpression(UnaryExpression *expression, T t) {
    auto expr1 = expression->expr1->acceptResultVisitor(*this, t);
    if (expr1 != expression->expr1) {
        return std::make_shared<UnaryExpression>(expression->operation, expr1);
    }
    return std::make_shared<UnaryExpression>(expression, t);
}

template<typename T>
std::shared_ptr<node> OptimizationVisitor<T>::visitValueExpression(ValueExpression *expression, T t) {
    return std::make_shared<ValueExpression>(expression, t);
}

template<typename T>
std::shared_ptr<node> OptimizationVisitor<T>::visitVariableExpression(VariableExpression *expression, T t) {
    return std::make_shared<VariableExpression>(expression, t);
}