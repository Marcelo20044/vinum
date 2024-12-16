#ifndef CONDITIONAL_EXPRESSION_H
#define CONDITIONAL_EXPRESSION_H

#include "../../ast.h"
#include <memory>
#include <string>

class ConditionalExpression : public Expression {
public:
    enum class Operator {
        EQUALS,
        NOT_EQUALS,
        LT,
        LTEQ,
        GT,
        GTEQ,
        AND,
        OR
    };

private:
    std::shared_ptr<Expression> expr1;
    std::shared_ptr<Expression> expr2;
    Operator operation;

public:
    ConditionalExpression(Operator operation, std::shared_ptr<Expression> expr1, std::shared_ptr<Expression> expr2);

    std::shared_ptr<Value> eval() override;

    static std::string operatorToString(Operator op);
};

#endif // CONDITIONAL_EXPRESSION_H
