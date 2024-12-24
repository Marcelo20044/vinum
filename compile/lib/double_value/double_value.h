#ifndef DOUBLE_VALUE_H
#define DOUBLE_VALUE_H

#include "../value.h"

class DoubleValue : public Value {
    double value;

public:
    static const DoubleValue ZERO;

    explicit DoubleValue(double value);

    short asShort() const override;

    int asInt() const override;

    long asLong() const override;

    bool asBoolean() const override;

    double asDouble() const override;

    std::string asString() const override;

    std::vector<std::shared_ptr<Value>> asArray() const override;

    void swap(Value &other) override;
};

#endif // DOUBLE_VALUE_H
