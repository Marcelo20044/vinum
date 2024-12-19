#include "short_value.h"
#include <string>

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
