#ifndef SHORT_VALUE_H
#define SHORT_VALUE_H

#include "../value.h"

class ShortValue : public Value {
    short value;

public:
    static const ShortValue ZERO;

    explicit ShortValue(short value);

    short asShort() const override;

    int asInt() const override;

    long asLong() const override;

    bool asBoolean() const override;

    double asDouble() const override;

    std::string asString() const override;

    std::vector<std::shared_ptr<Value>> asArray() const override;

    void swap(Value &other) override;
};


#endif //SHORT_VALUE_H
