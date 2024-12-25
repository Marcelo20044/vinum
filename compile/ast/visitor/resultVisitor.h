#ifndef VINUM_RESULTVISITOR_H
#define VINUM_RESULTVISITOR_H

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

template<typename R, typename T>
class ResultVisitor {
public:
    virtual ~ResultVisitor() = default;

    virtual R visit(AssignmentStatement *statement, T context) = 0;

    virtual R visit(BlockStatement *statement, T context) = 0;

    virtual R visit(ArrayInitializationStatement *statement, T context) = 0;

    virtual R visit(ArrayAssignmentStatement *statement, T context) = 0;

    virtual R visit(BreakStatement *statement, T context) = 0;

    virtual R visit(ContinueStatement *statement, T context) = 0;

    virtual R visit(ForStatement *statement, T context) = 0;

    virtual R visit(FunctionDefineStatement *statement, T context) = 0;

    virtual R visit(FunctionStatement *statement, T context) = 0;

    virtual R visit(IfStatement *statement, T context) = 0;

    virtual R visit(InitializationStatement *statement, T context) = 0;

    virtual R visit(PrintStatement *statement, T context) = 0;

    virtual R visit(ReturnStatement *statement, T context) = 0;

    virtual R visit(ArrayAccessExpression *expression, T context) = 0;

    virtual R visit(BinaryExpression *expression, T context) = 0;

    virtual R visit(ConditionalExpression *expression, T context) = 0;

    virtual R visit(FunctionalExpression *expression, T context) = 0;

    virtual R visit(UnaryExpression *expression, T context) = 0;

    virtual R visit(ValueExpression *expression, T context) = 0;

    virtual R visit(VariableExpression *expression, T context) = 0;
};

#endif //VINUM_RESULTVISITOR_H
