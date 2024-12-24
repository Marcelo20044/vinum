#include "array_value.h"

#include <utility>

ArrayValue::ArrayValue(size_t size, ValueType elemsType) : Value(ValueType::ARRAY), elemsType(elemsType), elements(size) {
}

ArrayValue::ArrayValue(ValueType elemsType, std::vector<std::shared_ptr<Value>> elements): Value(ValueType::ARRAY), elemsType(elemsType), elements(std::move(elements)) {
}

std::shared_ptr<Value> ArrayValue::get(size_t index) const {
    if (index >= elements.size()) {
        throw std::out_of_range("Array index out of bounds");
    }
    return elements[index];
}

void ArrayValue::set(size_t index, const std::shared_ptr<Value>& value) {
    if (index >= elements.size()) {
        throw std::out_of_range("Array index out of bounds");
    }
    elements[index] = asType(value, elemsType);
}

short ArrayValue::asShort() const {
    throw std::runtime_error("Cannot cast []" + getTypeString(elemsType) + " to stopka");
}

int ArrayValue::asInt() const {
    throw std::runtime_error("Cannot cast []" + getTypeString(elemsType) + " to pinta");
}

long ArrayValue::asLong() const {
    throw std::runtime_error("Cannot cast []" + getTypeString(elemsType) + " to kega");
}

bool ArrayValue::asBoolean() const {
    throw std::runtime_error("Cannot cast []" + getTypeString(elemsType) + " to redbool");
}

double ArrayValue::asDouble() const {
    throw std::runtime_error("Cannot cast []" + getTypeString(elemsType) + " to bubble");
}

std::string ArrayValue::asString() const {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < elements.size(); ++i) {
        oss << (elements[i] ? elements[i]->asString() : "null");
        if (i != elements.size() - 1) {
            oss << ", ";
        }
    }
    oss << "]";
    return oss.str();
}

std::vector<std::shared_ptr<Value>> ArrayValue::asArray() const {
    return elements;
}

ValueType ArrayValue::getElemsType() const {
    return elemsType;
}

size_t ArrayValue::getSize() const {
    return elements.size();
}

void ArrayValue::swap(Value &other) {
    auto* otherArray = dynamic_cast<ArrayValue*>(&other);
    if (!otherArray) {
        throw std::runtime_error("Cannot swap array with a non-array value");
    }

    if (elemsType != otherArray->elemsType) {
        throw std::runtime_error("Cannot swap arrays with different element types");
    }

    std::swap(elements, otherArray->elements);
}
