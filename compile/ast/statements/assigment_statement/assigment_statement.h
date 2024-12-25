#ifndef ASSIGNMENT_STATEMENT_H
#define ASSIGNMENT_STATEMENT_H

#include <string>
#include "../../ast.h"

class AssignmentStatement : public Statement {
public:
    std::string variable;
    std::shared_ptr<Expression> expression;

public:
    AssignmentStatement(std::string variable, const std::shared_ptr<Expression> &expression);

    void execute() override;

    std::shared_ptr<node> accept(visitor *visitor) override {
        return visitor->visitAssignmentStatement(this);
    }
};

#endif // ASSIGNMENT_STATEMENT_H
