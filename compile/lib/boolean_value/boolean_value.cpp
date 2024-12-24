#include "boolean_value.h"
#include <string>
#include <vector>

BooleanValue::BooleanValue(bool value) : Value(ValueType::BOOLEAN), value(value) {
}

short BooleanValue::asShort() const {
    return value;
}

int BooleanValue::asInt() const {
    return value;
}

long BooleanValue::asLong() const {
    return value;
}

bool BooleanValue::asBoolean() const {
    return value;
}

double BooleanValue::asDouble() const {
    return value;
}

std::string BooleanValue::asString() const {
    return std::to_string(value);
}

std::vector<std::shared_ptr<Value>> BooleanValue::asArray() const {
    throw std::runtime_error("Cannot convert redbool to array");
}

void BooleanValue::swap(Value &other) {
    auto* otherBoolean = dynamic_cast<BooleanValue*>(&other);
    if (!otherBoolean) {
        throw std::runtime_error("Cannot swap redbool with a non-redbool value");
    }
    std::swap(value, otherBoolean->value);
}
