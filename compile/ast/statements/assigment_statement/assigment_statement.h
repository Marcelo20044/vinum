#ifndef ASSIGNMENT_STATEMENT_H
#define ASSIGNMENT_STATEMENT_H

#include <string>
#include "../../ast.h"


class AssignmentStatement : public Statement {
private:
    std::string variable;
    Expression* expression;

public:
    AssignmentStatement(const std::string& variable, Expression* expression);

    void execute() override;

    ~AssignmentStatement();
};

#endif // ASSIGNMENT_STATEMENT_H
