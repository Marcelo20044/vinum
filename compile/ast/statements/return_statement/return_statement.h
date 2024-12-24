#ifndef RETURN_STATEMENT_H
#define RETURN_STATEMENT_H

#include <stdexcept>
#include "../../ast.h"

class ReturnStatement : public std::runtime_error, public Statement {
public:
    std::shared_ptr<Expression> expression;
    std::shared_ptr<Value> result;

public:
    explicit ReturnStatement(std::shared_ptr<Expression> expression);

    std::shared_ptr<Value> getResult() const;

    void execute() override;

    std::shared_ptr<node> accept(visitor *visitor) override {
        return visitor->visitReturnStatement(this);
    }
};

#endif // RETURN_STATEMENT_H
