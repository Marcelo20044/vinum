#ifndef AST_H
#define AST_H

#include "../lib/value.h"
#include "../lib/builtin/variables/variables.h"
#include "visitor/visitor.h"
#include "visitor/node.h"

class Expression : public node {
public:
    ~Expression() override = default;

    virtual std::shared_ptr<Value> eval() = 0;

    virtual std::shared_ptr<node> accept(visitor *visitor) = 0;
};

class Statement : public node {
public:
    ~Statement() override = default;

    virtual std::shared_ptr<node> accept(visitor *visitor) = 0;
};

#endif // AST_H
