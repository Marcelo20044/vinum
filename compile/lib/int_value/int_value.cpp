#include "int_value.h"
#include <string>
#include <vector>

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

std::vector<std::shared_ptr<Value>> IntValue::asArray() const {
    throw std::runtime_error("Cannot convert pinta to array");
}

void IntValue::swap(Value &other) {
    auto* otherInt = dynamic_cast<IntValue*>(&other);
    if (!otherInt) {
        throw std::runtime_error("Cannot swap pinta with a non-pinta value");
    }
    std::swap(value, otherInt->value);
}
