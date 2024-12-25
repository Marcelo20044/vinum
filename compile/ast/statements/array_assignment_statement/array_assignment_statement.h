#ifndef ARRAY_ASSIGNMENT_STATEMENT_H
#define ARRAY_ASSIGNMENT_STATEMENT_H

#include "../../ast.h"
#include <string>

class ArrayAssignmentStatement : public Statement {
public:
    std::string variable;
    std::shared_ptr<Expression> index;
    std::shared_ptr<Expression> expression;

    std::shared_ptr<Expression> getIndex() {return index;};

    ArrayAssignmentStatement(std::string variable,
                             const std::shared_ptr<Expression> &index,
                             const std::shared_ptr<Expression> &expression);

    void execute() override;
};

#endif // ARRAY_ASSIGNMENT_STATEMENT_H
