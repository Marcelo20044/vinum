#ifndef USER_DEFINED_FUNCTION_H
#define USER_DEFINED_FUNCTION_H

#include "../value.h"
#include "../function.h"
#include "../../ast/ast.h"
#include <string>
#include <vector>
#include <memory>

class UserFunction : public Function {
    std::vector<std::string> argNames;
    std::shared_ptr<Statement> body;

public:
    UserFunction(const std::vector<std::string> &argNames, std::shared_ptr<Statement> body);

    int getArgsCount() const;

    std::string getArgsName(int index) const;

    std::shared_ptr<Value> execute(const std::vector<std::shared_ptr<Value> > &args) override;
};

#endif // USER_DEFINED_FUNCTION_H
