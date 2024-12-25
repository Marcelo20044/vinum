#ifndef AST_H
#define AST_H

#include "../lib/value.h"
#include "../lib/builtin/variables/variables.h"

class Expression {
public:
    virtual ~Expression() = default;

    virtual std::shared_ptr<Value> eval() = 0;
};

class Statement {
public:
    virtual ~Statement() = default;

    virtual void execute() = 0;
};

#endif // AST_H
