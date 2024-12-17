#ifndef IF_STATEMENT_H
#define IF_STATEMENT_H

#include "../../ast.h"

class IfStatement : public Statement {
    std::shared_ptr<Expression> expression;
    std::shared_ptr<Statement> ifStatement;
    std::shared_ptr<Statement> elseStatement;

public:
    IfStatement(std::shared_ptr<Expression> expression,
                std::shared_ptr<Statement> ifStatement,
                std::shared_ptr<Statement> elseStatement);

    void execute() override;
};

#endif // IF_STATEMENT_H
