#ifndef VALUE_H
#define VALUE_H

#include <string>

class Value {
public:
    virtual ~Value() = default;

    virtual double asNumber() const = 0;

    virtual std::string asString() const = 0;
};

#endif // VALUE_H
