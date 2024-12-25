#ifndef BLOCK_STATEMENT_H
#define BLOCK_STATEMENT_H

#include <vector>
#include <sstream>
#include "../../ast.h"

class BlockStatement : public Statement {
    std::vector<std::shared_ptr<Statement> > statements;

public:
    void add(const std::shared_ptr<Statement> &statement);

    void execute() override;

    std::vector<std::shared_ptr<Statement>> getStatements();
};

#endif // BLOCK_STATEMENT_H
