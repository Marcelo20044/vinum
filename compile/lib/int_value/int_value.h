#ifndef INT_VALUE_H
#define INT_VALUE_H

#include "../value.h"

class IntValue : public Value {
    int value;

public:
    static const IntValue ZERO;

    explicit IntValue(int value);

    short asShort() const override;

    int asInt() const override;

    long asLong() const override;

    bool asBoolean() const override;

    double asDouble() const override;

    std::string asString() const override;

    std::vector<std::shared_ptr<Value>> asArray() const override;

    void swap(Value &other) override;
};


#endif //INT_VALUE_H
