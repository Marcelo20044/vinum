#ifndef BLOCK_STATEMENT_H
#define BLOCK_STATEMENT_H

#include <vector>
#include <sstream>
#include "../../ast.h"

class BlockStatement : public Statement {
public:
    std::vector<std::shared_ptr<Statement> > statements;

public:
    void add(const std::shared_ptr<Statement> &statement);

    void execute() override;

    std::vector<std::shared_ptr<Statement>> getStatements();

    std::shared_ptr<node> accept(visitor *visitor) override {
        return visitor->visitBlockStatement(this);
    }
};

#endif // BLOCK_STATEMENT_H
