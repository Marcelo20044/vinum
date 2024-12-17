#ifndef FUNCTIONAL_EXPRESSION_H
#define FUNCTIONAL_EXPRESSION_H

#include <vector>
#include <string>
#include "../../ast.h"
#include "../../../lib/function.h"

class FunctionalExpression : public Expression {
    std::string name;
    std::vector<std::shared_ptr<Expression> > arguments;

public:
    explicit FunctionalExpression(std::string name);

    FunctionalExpression(std::string name, const std::vector<std::shared_ptr<Expression> > &args);

    void addArgument(std::shared_ptr<Expression> arg);

    std::shared_ptr<Value> eval() override;
};

#endif // FUNCTIONAL_EXPRESSION_H
