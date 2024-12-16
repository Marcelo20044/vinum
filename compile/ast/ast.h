#ifndef AST_H
#define AST_H

#include <string>
#include "../lib/value.h"
#include "../lib/variables/variables.h"
#include "../lib/number_value/number_value.h"
#include "../lib/string_value/string_value.h"

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