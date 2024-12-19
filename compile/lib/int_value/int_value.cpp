#include "int_value.h"
#include <string>

const IntValue IntValue::ZERO(0);

IntValue::IntValue(int value) : Value(ValueType::INT), value(value) {
}

short IntValue::asShort() const {
    if (value < std::numeric_limits<short>::min() || value > std::numeric_limits<short>::max()) {
        throw std::runtime_error("Cannot convert pinta to stopka: value out of bounds");
    }
    return static_cast<short>(value);
}

int IntValue::asInt() const {
    return value;
}

long IntValue::asLong() const {
    return value;
}

bool IntValue::asBoolean() const {
    return value;
}

double IntValue::asDouble() const {
    return value;
}

std::string IntValue::asString() const {
    return std::to_string(value);
}
