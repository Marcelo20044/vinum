#ifndef PRINT_STATEMENT_H
#define PRINT_STATEMENT_H

#include "../../ast.h"

class PrintStatement : public Statement {
public:
    std::shared_ptr<Expression> expression;

public:
    explicit PrintStatement(std::shared_ptr<Expression> expression);

    void execute() override;

    std::shared_ptr<node> accept(visitor *visitor) override {
        return visitor->visitPrintStatement(this);
    }
};

#endif // PRINT_STATEMENT_H
