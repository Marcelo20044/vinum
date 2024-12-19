#ifndef USER_DEFINED_FUNCTION_H
#define USER_DEFINED_FUNCTION_H

#include "../value.h"
#include "../function.h"
#include "../../ast/ast.h"
#include <string>
#include <vector>

struct Argument {
    ValueType type;
    std::string name;

    Argument(ValueType type, std::string name) : type(type), name(std::move(name)) {}
};

class UserFunction : public Function {
    ValueType returnType;
    std::vector<std::shared_ptr<Argument>> args;
    std::shared_ptr<Statement> body;

public:
    UserFunction(ValueType returnType, std::vector<std::shared_ptr<Argument>> &args, std::shared_ptr<Statement> body);

    int getArgsCount() const;

    std::string getArgsName(int index) const;

    std::shared_ptr<Value> execute(const std::vector<std::shared_ptr<Value> > &args) override;
};

#endif // USER_DEFINED_FUNCTION_H
