#include "long_value.h"
#include <string>
#include <vector>

const LongValue LongValue::ZERO(0);

LongValue::LongValue(long value) : Value(ValueType::LONG), value(value) {
}

short LongValue::asShort() const {
    if (value < std::numeric_limits<short>::min() || value > std::numeric_limits<short>::max()) {
        throw std::runtime_error("Cannot convert kega to stopka: value out of bounds");
    }
    return static_cast<short>(value);
}

int LongValue::asInt() const {
    if (value < std::numeric_limits<int>::min() || value > std::numeric_limits<int>::max()) {
        throw std::runtime_error("Cannot convert kega to stopka: value out of bounds");
    }
    return static_cast<int>(value);
}

long LongValue::asLong() const {
    return value;
}

bool LongValue::asBoolean() const {
     return value;
}

double LongValue::asDouble() const {
    return static_cast<double>(value);
}

std::string LongValue::asString() const {
    return std::to_string(value);
}

std::vector<std::shared_ptr<Value>> LongValue::asArray() const {
    throw std::runtime_error("Cannot convert kega to array");
}

void LongValue::swap(Value &other) {
    auto* otherKega = dynamic_cast<LongValue*>(&other);
    if (!otherKega) {
        throw std::runtime_error("Cannot swap kega with a non-kega value");
    }
    std::swap(value, otherKega->value);
}
