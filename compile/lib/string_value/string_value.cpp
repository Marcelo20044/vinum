#include "string_value.h"
#include <string>
#include <sstream>
#include <vector>

StringValue::StringValue(std::string  value) : Value(ValueType::STRING), value(std::move(value)) {
}

short StringValue::asShort() const {
    try {
        return static_cast<short>(std::stoi(value));
    } catch (const std::invalid_argument&) {
        throw std::runtime_error("Cannot convert sdrink \"" + value + "\" to stopka");
    } catch (const std::out_of_range&) {
        throw std::runtime_error("Cannot convert sdrink \"" + value + "\" to stopka: value out of bounds");
    }
}

int StringValue::asInt() const {
    try {
        return std::stoi(value);
    } catch (const std::invalid_argument&) {
        throw std::runtime_error("Cannot convert sdrink \"" + value + "\" to pinta");
    } catch (const std::out_of_range&) {
        throw std::runtime_error("Cannot convert sdrink \"" + value + "\" to pinta: value out of bounds");
    }
}

long StringValue::asLong() const {
    try {
        return std::stol(value);
    } catch (const std::invalid_argument&) {
        throw std::runtime_error("Cannot convert sdrink \"" + value + "\" to kega");
    } catch (const std::out_of_range&) {
        throw std::runtime_error("Cannot convert sdrink \"" + value + "\" to kega: value out of bounds");
    }
}

bool StringValue::asBoolean() const {
    throw std::runtime_error("Cannot convert sdrink to redbool");
}

double StringValue::asDouble() const {
    try {
        return std::stod(value);
    } catch (const std::invalid_argument&) {
        throw std::runtime_error("Cannot convert sdrink \"" + value + "\" to bubble");
    } catch (const std::out_of_range&) {
        throw std::runtime_error("Cannot convert sdrink \"" + value + "\" to bubble: value out of bounds");
    }
}

std::string StringValue::asString() const {
    return value;
}

std::vector<std::shared_ptr<Value>> StringValue::asArray() const {
    throw std::runtime_error("Cannot convert sdrink to array");
}

void StringValue::swap(Value &other) {
    auto* otherString = dynamic_cast<StringValue*>(&other);
    if (!otherString) {
        throw std::runtime_error("Cannot swap sdrink with a non-sdrink value");
    }
    std::swap(value, otherString->value);
}
