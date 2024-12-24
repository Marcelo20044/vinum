#ifndef BOOLEAN_VALUE_H
#define BOOLEAN_VALUE_H

#include "../value.h"

class BooleanValue : public Value {
    bool value;

public:
    explicit BooleanValue(bool value);

    short asShort() const override;

    int asInt() const override;

    long asLong() const override;

    bool asBoolean() const override;

    double asDouble() const override;

    std::string asString() const override;

    std::vector<std::shared_ptr<Value>> asArray() const override;

    void swap(Value &other) override;
};


#endif //BOOLEAN_VALUE_H
