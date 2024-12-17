#ifndef FUNCTION_H
#define FUNCTION_H

#include "value.h"
#include <vector>

class Function {
public:
    virtual ~Function() = default;

    virtual std::shared_ptr<Value> execute(const std::vector<std::shared_ptr<Value> > &args) = 0;
};

#endif // FUNCTION_H
