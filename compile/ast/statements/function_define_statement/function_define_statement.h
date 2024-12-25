#ifndef FUNCTION_DEFINE_STATEMENT_H
#define FUNCTION_DEFINE_STATEMENT_H

#include <string>
#include <vector>
#include "../../ast.h"
#include "../../../lib/user_function/user_function.h"

struct Argument;

class FunctionDefineStatement : public Statement {
public:
    std::string name;
    ValueType returnType;
    std::vector<std::shared_ptr<Argument> > args;
    std::shared_ptr<Statement> body;

public:
    FunctionDefineStatement(std::string name, ValueType returnType,
                            const std::vector<std::shared_ptr<Argument> > &args,
                            std::shared_ptr<Statement> body);

    void execute() override;

    std::shared_ptr<node> accept(visitor *visitor) override {
        return visitor->visitFunctionDefineStatement(this);
    }
};

#endif // FUNCTION_DEFINE_STATEMENT_H
