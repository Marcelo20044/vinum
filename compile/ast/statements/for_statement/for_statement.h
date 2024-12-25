#ifndef FOR_STATEMENT_H
#define FOR_STATEMENT_H

#include "../../ast.h"

class ForStatement : public Statement {
public:
    std::shared_ptr<Statement> initialization;
    std::shared_ptr<Expression> termination;
    std::shared_ptr<Statement> increment;
    std::shared_ptr<Statement> statement;

public:
    ForStatement(std::shared_ptr<Statement> initialization,
                 std::shared_ptr<Expression> termination,
                 std::shared_ptr<Statement> increment,
                 std::shared_ptr<Statement> block);

    void execute() override;
};

#endif // FOR_STATEMENT_H
