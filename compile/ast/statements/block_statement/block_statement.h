#ifndef BLOCK_STATEMENT_H
#define BLOCK_STATEMENT_H

#include <vector>
#include <string>
#include <sstream>
#include "../../ast.h"

class BlockStatement : public Statement {
private:
    std::vector<Statement*> statements;

public:
    BlockStatement() = default;

    void add(Statement* statement);

    void execute() override;

    ~BlockStatement();
};

#endif // BLOCK_STATEMENT_H
