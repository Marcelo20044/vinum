#include "value.h"

#include <unordered_map>

#include "boolean_value/boolean_value.h"
#include "double_value/double_value.h"
#include "int_value/int_value.h"
#include "long_value/long_value.h"
#include "short_value/short_value.h"
#include "string_value/string_value.h"

const std::unordered_map<std::string, ValueType> Value::types = {
    {"stopka", ValueType::SHORT},
    {"pinta", ValueType::INT},
    {"kega", ValueType::LONG},
    {"bubble", ValueType::DOUBLE},
    {"sdrink", ValueType::STRING},
    {"redbool", ValueType::BOOLEAN},
    {"void", ValueType::VOID}
};

bool Value::typeExists(const std::string &type) {
    return types.contains(type);
}

ValueType Value::getType(const std::string &type) {
    if (!types.contains(type)) {
        throw std::runtime_error("Unknown type: " + type);
    }
    return types.at(type);
}

std::shared_ptr<Value> Value::asType(std::shared_ptr<Value> value, ValueType type) {
    switch (type) {
        case ValueType::SHORT:
            return std::make_shared<ShortValue>(value->asShort());
        case ValueType::INT:
            return std::make_shared<IntValue>(value->asInt());
        case ValueType::LONG:
            return std::make_shared<LongValue>(value->asLong());
        case ValueType::DOUBLE:
            return std::make_shared<DoubleValue>(value->asDouble());
        case ValueType::BOOLEAN:
            return std::make_shared<BooleanValue>(value->asBoolean());
        case ValueType::STRING:
            return std::make_shared<StringValue>(value->asString());
    }
    throw std::runtime_error("Unknown value type");
}
