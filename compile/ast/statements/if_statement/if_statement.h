#ifndef IF_STATEMENT_H
#define IF_STATEMENT_H

#include "../../ast.h"
#include "../../visitor/visitor.h"

class IfStatement : public Statement {
public:
    std::shared_ptr<Expression> expression;
    std::shared_ptr<Statement> ifStatement;
    std::shared_ptr<Statement> elseStatement;

public:
    IfStatement(std::shared_ptr<Expression> expression,
                std::shared_ptr<Statement> ifStatement,
                std::shared_ptr<Statement> elseStatement);

    void execute() override;

    std::shared_ptr<node> accept(visitor *visitor) override {
        return visitor->visitIfStatement(this);
    }
};

#endif // IF_STATEMENT_H
