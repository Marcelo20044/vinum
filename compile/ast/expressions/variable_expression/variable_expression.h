#ifndef VARIABLE_EXPRESSION_H
#define VARIABLE_EXPRESSION_H

#include "../../ast.h"
#include <memory>
#include <string>

class VariableExpression : public Expression {
private:
    std::string name;

public:
    explicit VariableExpression(const std::string& name);

    std::shared_ptr<Value> eval() override;
};

#endif // VARIABLE_EXPRESSION_H
