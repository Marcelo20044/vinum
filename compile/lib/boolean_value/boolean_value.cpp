#include "boolean_value.h"
#include <string>

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
