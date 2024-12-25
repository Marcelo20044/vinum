#ifndef ARRAY_ACCESS_EXPRESSION_H
#define ARRAY_ACCESS_EXPRESSION_H

#include <utility>

#include "../../ast.h"

class ArrayAccessExpression : public Expression {
public:
    std::string variable;
    std::shared_ptr<Expression> index;

    ArrayAccessExpression(std::string variable, const std::shared_ptr<Expression> &index);

    std::shared_ptr<Value> eval() override;
};



#endif //ARRAY_ACCESS_EXPRESSION_H
