#ifndef NUMBER_VALUE_H
#define NUMBER_VALUE_H

#include "../value.h"

class NumberValue : public Value {
    double value;

public:
    static const NumberValue ZERO;

    explicit NumberValue(double value);

    double asNumber() const override;
    std::string asString() const override;
};

#endif // NUMBER_VALUE_H
