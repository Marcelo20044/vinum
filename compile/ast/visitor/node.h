#ifndef VINUM_NODE_H
#define VINUM_NODE_H


#include "resultVisitor.h"
#include "visitor.h"

class node {
public:
    virtual ~node() = default;

//    virtual std::shared_ptr<node> accept(visitor *visitor) = 0;

    template<typename R, typename T>
    R acceptResultVisitor(ResultVisitor<R, T> &visitor, T input) {
        return visitor.visit(this, input);
    }
};


#endif //VINUM_NODE_H
