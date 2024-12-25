#ifndef VINUM_VISITOR_H
#define VINUM_VISITOR_H


class AssignmentStatement;

class BlockStatement;

class ArrayInitializationStatement;

class ArrayAssignmentStatement;

class BreakStatement;

class ContinueStatement;

class ForStatement;

class FunctionDefineStatement;

class FunctionStatement;

class IfStatement;

class InitializationStatement;

class PrintStatement;

class ReturnStatement;

class ArrayAccessExpression;

class BinaryExpression;

class ConditionalExpression;

class FunctionalExpression;

class UnaryExpression;

class ValueExpression;

class VariableExpression;


class visitor {
public:
    virtual ~visitor() = default;

    virtual void visitAssignmentStatement(AssignmentStatement *statement) = 0;

    virtual void visitBlockStatement(BlockStatement *statement) = 0;

    virtual void visitArrayInitializationStatement(ArrayInitializationStatement *statement) = 0;

    virtual void visitArrayAssignmentStatement(ArrayAssignmentStatement *statement) = 0;

    virtual void visitBreakStatement(BreakStatement *statement) = 0;

    virtual void visitContinueStatement(ContinueStatement *statement) = 0;

    virtual void visitForStatement(ForStatement *statement) = 0;

    virtual void visitFunctionDefineStatement(FunctionDefineStatement *statement) = 0;

    virtual void visitFunctionStatement(FunctionStatement *statement) = 0;

    virtual void visitIfStatement(IfStatement *statement) = 0;

    virtual void visitInitializationStatement(InitializationStatement *statement) = 0;

    virtual void visitPrintStatement(PrintStatement *statement) = 0;

    virtual void visitReturnStatement(ReturnStatement *statement) = 0;

    virtual void visitArrayAccessExpression(ArrayAccessExpression *expression) = 0;

    virtual void visitBinaryExpression(BinaryExpression *expression) = 0;

    virtual void visitConditionalExpression(ConditionalExpression *expression) = 0;

    virtual void visitFunctionalExpression(FunctionalExpression *expression) = 0;

    virtual void visitUnaryExpression(UnaryExpression *expression) = 0;

    virtual void visitValueExpression(ValueExpression *expression) = 0;

    virtual void visitVariableExpression(VariableExpression *expression) = 0;
};


#endif //VINUM_VISITOR_H
