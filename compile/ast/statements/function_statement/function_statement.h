#ifndef FUNCTION_STATEMENT_H
#define FUNCTION_STATEMENT_H

#include "../../ast.h"
#include "../../expressions/functional_expression/functional_expression.h"

class FunctionStatement : public Statement {
public:
    std::shared_ptr<FunctionalExpression> function;

public:
    explicit FunctionStatement(std::shared_ptr<FunctionalExpression> function);

    void execute() override;
};

#endif // FUNCTION_STATEMENT_H
