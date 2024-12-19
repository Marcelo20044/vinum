#ifndef STRING_VALUE_H
#define STRING_VALUE_H

#include "../value.h"
#include <string>

class StringValue : public Value {
    std::string value;

public:
    explicit StringValue(std::string value);

    short asShort() const override;

    int asInt() const override;

    long asLong() const override;

    double asDouble() const override;

    bool asBoolean() const override;

    std::string asString() const override;
};

#endif // STRING_VALUE_H
