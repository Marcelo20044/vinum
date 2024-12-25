#ifndef VINUM_OPTIMIZABLE_H
#define VINUM_OPTIMIZABLE_H

#include "../visitor/node.h"
#include <memory>

class Optimizable {
public:
    virtual ~Optimizable() = default;

    virtual std::shared_ptr<node> optimize(node *node) = 0;
};


#endif //VINUM_OPTIMIZABLE_H
