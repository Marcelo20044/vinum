#ifndef FUNCTION_DEFINE_STATEMENT_H
#define FUNCTION_DEFINE_STATEMENT_H

#include <string>
#include <vector>
#include "../../ast.h"

class FunctionDefineStatement : public Statement {
    std::string name;
    std::vector<std::string> argNames;
    std::shared_ptr<Statement> body;

public:
    FunctionDefineStatement(std::string name, const std::vector<std::string> &argNames,
                            std::shared_ptr<Statement> body);

    void execute() override;
};

#endif // FUNCTION_DEFINE_STATEMENT_H
