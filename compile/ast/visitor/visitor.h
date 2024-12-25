#ifndef VINUM_VISITOR_H
#define VINUM_VISITOR_H


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
#include "node.h"

class visitor {
public:
    virtual ~visitor() = default;

    virtual std::shared_ptr<node> visitAssignmentStatement(AssignmentStatement *statement) = 0;

    virtual std::shared_ptr<node> visitBlockStatement(BlockStatement *statement) = 0;

    virtual std::shared_ptr<node> visitArrayInitializationStatement(ArrayInitializationStatement *statement) = 0;

    virtual std::shared_ptr<node> visitArrayAssignmentStatement(ArrayAssignmentStatement *statement) = 0;

    virtual std::shared_ptr<node> visitBreakStatement(BreakStatement *statement) = 0;

    virtual std::shared_ptr<node> visitContinueStatement(ContinueStatement *statement) = 0;

    virtual std::shared_ptr<node> visitForStatement(ForStatement *statement) = 0;

    virtual std::shared_ptr<node> visitFunctionDefineStatement(FunctionDefineStatement *statement) = 0;

    virtual std::shared_ptr<node> visitFunctionStatement(FunctionStatement *statement) = 0;

    virtual std::shared_ptr<node> visitIfStatement(IfStatement *statement) = 0;

    virtual std::shared_ptr<node> visitInitializationStatement(InitializationStatement *statement) = 0;

    virtual std::shared_ptr<node> visitPrintStatement(PrintStatement *statement) = 0;

    virtual std::shared_ptr<node> visitReturnStatement(ReturnStatement *statement) = 0;

    virtual std::shared_ptr<node> visitArrayAccessExpression(ArrayAccessExpression *expression) = 0;

    virtual std::shared_ptr<node> visitBinaryExpression(BinaryExpression *expression) = 0;

    virtual std::shared_ptr<node> visitConditionalExpression(ConditionalExpression *expression) = 0;

    virtual std::shared_ptr<node> visitFunctionalExpression(FunctionalExpression *expression) = 0;

    virtual std::shared_ptr<node> visitUnaryExpression(UnaryExpression *expression) = 0;

    virtual std::shared_ptr<node> visitValueExpression(ValueExpression *expression) = 0;

    virtual std::shared_ptr<node> visitVariableExpression(VariableExpression *expression) = 0;
};


#endif //VINUM_VISITOR_H
