#include "short_value.h"
#include <string>
#include <vector>

const ShortValue ShortValue::ZERO(0);

ShortValue::ShortValue(short value) : Value(ValueType::SHORT), value(value) {
}

short ShortValue::asShort() const {
    return value;
}

int ShortValue::asInt() const {
    return value;
}

long ShortValue::asLong() const {
    return value;
}

bool ShortValue::asBoolean() const {
    return value;
}

double ShortValue::asDouble() const {
    return value;
}

std::string ShortValue::asString() const {
    return std::to_string(value);
}

std::vector<std::shared_ptr<Value>> ShortValue::asArray() const {
    throw std::runtime_error("Cannot convert sotopka to array");
}

void ShortValue::swap(Value &other) {
    auto* otherShort = dynamic_cast<ShortValue*>(&other);
    if (!otherShort) {
        throw std::runtime_error("Cannot swap stopka with a non-stopka value");
    }
    std::swap(value, otherShort->value);
}
