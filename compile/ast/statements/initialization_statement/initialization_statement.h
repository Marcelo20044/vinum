#ifndef INITIALIZATION_STATEMENT_H
#define INITIALIZATION_STATEMENT_H

#include "../../ast.h"

class InitializationStatement : public Statement {
public:
    ValueType varType;
    std::string varName;
    std::shared_ptr<Expression> expression;

public:
    InitializationStatement(ValueType varType, std::string variable, const std::shared_ptr<Expression> &expression);

    void execute() override;

    std::shared_ptr<node> accept(visitor *visitor) override {
        return visitor->visitInitializtionStatement(this);
    }
};


#endif //INITIALIZATION_STATEMENT_H
