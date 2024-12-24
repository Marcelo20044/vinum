#ifndef VINUM_NODE_H
#define VINUM_NODE_H


#include "visitor.h"
#include "resultVisitor.h"

class node {
public:
    virtual ~node() = default;

    virtual std::shared_ptr<node> accept(visitor *visitor) = 0;

    template<typename R, typename T>
    R accept(ResultVisitor<R, T> &visitor, T input);
};

template<typename R, typename T>
R node::accept(ResultVisitor<R, T> &visitor, T input) {
    return nullptr;
}


#endif //VINUM_NODE_H
