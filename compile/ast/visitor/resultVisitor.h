#ifndef VINUM_RESULTVISITOR_H
#define VINUM_RESULTVISITOR_H

#include "../statements/assigment_statement/assigment_statement.h"
#include "../statements/block_statement/block_statement.h"
#include "../statements/break_statement/break_statement.h"
#include "../statements/continue_statement/continue_statement.h"
#include "../statements/for_statement/for_statement.h"
#include "../statements/function_define_statement/function_define_statement.h"
#include "../statements/function_statement/function_statement.h"
#include "../statements/if_statement/if_statement.h"
#include "../statements/initialization_statement/initialization_statement.h"
#include "../statements/print_statement/print_statement.h"
#include "../statements/return_statement/return_statement.h"
#include "../statements/array_initialization_statement/array_initialization_statement.h"
#include "../statements/array_assignment_statement/array_assignment_statement.h"
#include "../expressions/array_access_expression/array_access_expression.h"
#include "../expressions/binary_expression/binary_expression.h"
#include "../expressions/conditional_expression/conditional_expression.h"
#include "../expressions/unary_expression/unary_expression.h"
#include "../expressions/value_expression/value_expression.h"
#include "../expressions/variable_expression/variable_expression.h"

template<typename R, typename T>
class ResultVisitor {
public:
    virtual ~ResultVisitor() = default;

    virtual R visitAssignmentStatement(AssignmentStatement *statement, T context) = 0;

    virtual R visitBlockStatement(BlockStatement *statement, T context) = 0;

    virtual R visitArrayInitializationStatement(ArrayInitializationStatement *statement, T context) = 0;

    virtual R visitArrayAssignmentStatement(ArrayAssignmentStatement *statement, T context) = 0;

    virtual R visitBreakStatement(BreakStatement *statement, T context) = 0;

    virtual R visitContinueStatement(ContinueStatement *statement, T context) = 0;

    virtual R visitForStatement(ForStatement *statement, T context) = 0;

    virtual R visitFunctionDefineStatement(FunctionDefineStatement *statement, T context) = 0;

    virtual R visitFunctionStatement(FunctionStatement *statement, T context) = 0;

    virtual R visitIfStatement(IfStatement *statement, T context) = 0;

    virtual R visitInitializationStatement(InitializationStatement *statement, T context) = 0;

    virtual R visitPrintStatement(PrintStatement *statement, T context) = 0;

    virtual R visitReturnStatement(ReturnStatement *statement, T context) = 0;

    virtual R visitArrayAccessExpression(ArrayAccessExpression *expression, T context) = 0;

    virtual R visitBinaryExpression(BinaryExpression *expression, T context) = 0;

    virtual R visitConditionalExpression(ConditionalExpression *expression, T context) = 0;

    virtual R visitFunctionalExpression(FunctionalExpression *expression, T context) = 0;

    virtual R visitUnaryExpression(UnaryExpression *expression, T context) = 0;

    virtual R visitValueExpression(ValueExpression *expression, T context) = 0;

    virtual R visitVariableExpression(VariableExpression *expression, T context) = 0;
};

#endif //VINUM_RESULTVISITOR_H
