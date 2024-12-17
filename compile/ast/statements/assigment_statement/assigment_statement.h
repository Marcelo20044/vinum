#ifndef ASSIGNMENT_STATEMENT_H
#define ASSIGNMENT_STATEMENT_H

#include <string>
#include "../../ast.h"

class AssignmentStatement : public Statement {
    std::string variable;
    std::shared_ptr<Expression> expression;

public:
    AssignmentStatement(std::string variable, const std::shared_ptr<Expression> &expression);

    void execute() override;
};

#endif // ASSIGNMENT_STATEMENT_H
