#ifndef CONTINUE_STATEMENT_H
#define CONTINUE_STATEMENT_H

#include <stdexcept>
#include "../../ast.h"

class ContinueStatement : public std::runtime_error, public Statement {
public:
    ContinueStatement();

    void execute() override;

    std::shared_ptr<node> accept(visitor *visitor) override {
        return visitor->visitContinueStatement(this);
    }
};

#endif // CONTINUE_STATEMENT_H
