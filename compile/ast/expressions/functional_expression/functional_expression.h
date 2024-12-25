#ifndef FUNCTIONAL_EXPRESSION_H
#define FUNCTIONAL_EXPRESSION_H

#include <vector>
#include <string>
#include "../../ast.h"
#include "../../../lib/function.h"

class FunctionalExpression : public Expression {
public:
    std::string name;
    std::vector<std::shared_ptr<Expression> > arguments;

public:
    explicit FunctionalExpression(std::string name);

    FunctionalExpression(std::string name, const std::vector<std::shared_ptr<Expression> > &args);

    void addArgument(std::shared_ptr<Expression> arg);

    std::shared_ptr<Value> eval() override;

    std::shared_ptr<Expression> optimize() override;

    std::shared_ptr<node> accept(visitor *visitor) override {
        return visitor->visitFunctionalExpression(this);
    }
};

#endif // FUNCTIONAL_EXPRESSION_H
