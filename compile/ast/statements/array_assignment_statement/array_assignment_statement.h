#ifndef ARRAY_ASSIGNMENT_STATEMENT_H
#define ARRAY_ASSIGNMENT_STATEMENT_H

#include "../../ast.h"
#include <string>

class ArrayAssignmentStatement : public Statement {
    std::string variable;
    std::shared_ptr<Expression> index;
    std::shared_ptr<Expression> expression;

public:
    ArrayAssignmentStatement(std::string variable,
                             const std::shared_ptr<Expression> &index,
                             const std::shared_ptr<Expression> &expression);

    void execute() override;
};

#endif // ARRAY_ASSIGNMENT_STATEMENT_H
