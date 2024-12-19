#ifndef FUNCTION_DEFINE_STATEMENT_H
#define FUNCTION_DEFINE_STATEMENT_H

#include <string>
#include <vector>
#include "../../ast.h"

struct Argument;

class FunctionDefineStatement : public Statement {
    std::string name;
    ValueType returnType;
    std::vector<std::shared_ptr<Argument> > args;
    std::shared_ptr<Statement> body;

public:
    FunctionDefineStatement(std::string name, ValueType returnType,
                            const std::vector<std::shared_ptr<Argument> > &args,
                            std::shared_ptr<Statement> body);

    void execute() override;
};

#endif // FUNCTION_DEFINE_STATEMENT_H
