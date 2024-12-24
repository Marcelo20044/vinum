#ifndef LONG_VALUE_H
#define LONG_VALUE_H

#include "../value.h"

class LongValue : public Value {
    long value;

public:
    static const LongValue ZERO;

    explicit LongValue(long value);

    short asShort() const override;

    int asInt() const override;

    long asLong() const override;

    bool asBoolean() const override;

    double asDouble() const override;

    std::string asString() const override;

    std::vector<std::shared_ptr<Value>> asArray() const override;

    void swap(Value &other) override;
};


#endif //LONG_VALUE_H
