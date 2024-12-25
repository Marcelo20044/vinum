#ifndef VINUM_OPTIMIZATIONVISITOR_H
#define VINUM_OPTIMIZATIONVISITOR_H

#include "../resultVisitor.h"

template<typename T>
class OptimizationVisitor : public ResultVisitor<std::shared_ptr<node>, T> {
public:
    virtual ~OptimizationVisitor() = default;

    virtual std::shared_ptr<node> visitAssignmentStatement(AssignmentStatement *statement, T t) = 0;

    virtual std::shared_ptr<node> visitBlockStatement(BlockStatement *statement, T t) = 0;

    virtual std::shared_ptr<node> visitArrayInitializationStatement(ArrayInitializationStatement *statement, T t) = 0;

    virtual std::shared_ptr<node> visitArrayAssignmentStatement(ArrayAssignmentStatement *statement, T t) = 0;

    virtual std::shared_ptr<node> visitBreakStatement(BreakStatement *statement, T t) = 0;

    virtual std::shared_ptr<node> visitContinueStatement(ContinueStatement *statement, T t) = 0;

    virtual std::shared_ptr<node> visitForStatement(ForStatement *statement, T t) = 0;

    virtual std::shared_ptr<node> visitFunctionDefineStatement(FunctionDefineStatement *statement, T t);

    virtual std::shared_ptr<node> visitFunctionStatement(FunctionStatement *statement, T t) = 0;

    virtual std::shared_ptr<node> visitIfStatement(IfStatement *statement, T t) = 0;

    virtual std::shared_ptr<node> visitInitializationStatement(InitializationStatement *statement, T t) = 0;

    virtual std::shared_ptr<node> visitPrintStatement(PrintStatement *statement, T t) = 0;

    virtual std::shared_ptr<node> visitReturnStatement(ReturnStatement *statement, T t) = 0;

    virtual std::shared_ptr<node> visitArrayAccessExpression(ArrayAccessExpression *expression, T t) = 0;

    virtual std::shared_ptr<node> visitBinaryExpression(BinaryExpression *expression, T t) = 0;

    virtual std::shared_ptr<node> visitConditionalExpression(ConditionalExpression *expression, T t) = 0;

    virtual std::shared_ptr<node> visitFunctionalExpression(FunctionalExpression *expression, T t) = 0;

    virtual std::shared_ptr<node> visitUnaryExpression(UnaryExpression *expression, T t) = 0;

    virtual std::shared_ptr<node> visitValueExpression(ValueExpression *expression, T t) = 0;

    virtual std::shared_ptr<node> visitVariableExpression(VariableExpression *expression, T t) = 0;
};


#endif //VINUM_OPTIMIZATIONVISITOR_H
