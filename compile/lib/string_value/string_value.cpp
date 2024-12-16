#include "string_value.h"
#include <string>
#include <sstream>

StringValue::StringValue(const std::string& value) : value(value) {}

double StringValue::asNumber() const {
    try {
        return std::stod(value);
    } catch (const std::invalid_argument&) {
        return 0.0;
    }
}

std::string StringValue::asString() const {
    return value;
}
