#include "number_value.h"
#include <string>

const NumberValue NumberValue::ZERO(0);

NumberValue::NumberValue(double value) : value(value) {}

double NumberValue::asNumber() const {
    return value;
}

std::string NumberValue::asString() const {
    return std::to_string(value);
}
