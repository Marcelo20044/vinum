#include "double_value.h"
#include <string>

const DoubleValue DoubleValue::ZERO(0);

DoubleValue::DoubleValue(double value) : Value(ValueType::DOUBLE), value(value) {
}

short DoubleValue::asShort() const {
    if (value < std::numeric_limits<short>::min() || value > std::numeric_limits<short>::max()) {
        throw std::overflow_error("Value is out of range for a short");
    }
    return static_cast<short>(std::round(value));
}

int DoubleValue::asInt() const {
    if (value < std::numeric_limits<int>::min() || value > std::numeric_limits<int>::max()) {
        throw std::overflow_error("Value is out of range for an int");
    }
    return static_cast<int>(std::round(value));
}

long DoubleValue::asLong() const {
    if (value < static_cast<double>(std::numeric_limits<long>::min()) ||
        value > static_cast<double>(std::numeric_limits<long>::max())) {
        throw std::overflow_error("Value is out of range for a long");
    }

    return static_cast<long>(std::round(value));
}

bool DoubleValue::asBoolean() const {
    return value == 0.0;
}

double DoubleValue::asDouble() const {
    return value;
}

std::string DoubleValue::asString() const {
    return std::to_string(value);
}
