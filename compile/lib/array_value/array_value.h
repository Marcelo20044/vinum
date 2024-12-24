#ifndef ARRAY_VALUE_H
#define ARRAY_VALUE_H

#include "../value.h"
#include <vector>
#include <sstream>

class ArrayValue : public Value {
    ValueType elemsType;
    std::vector<std::shared_ptr<Value> > elements;

public:
    explicit ArrayValue(size_t size, ValueType elemsType);

    explicit ArrayValue(ValueType elemsType, std::vector<std::shared_ptr<Value> > elements);

    std::shared_ptr<Value> get(size_t index) const;

    void set(size_t index, const std::shared_ptr<Value> &value);

    short asShort() const override;

    int asInt() const override;

    long asLong() const override;

    double asDouble() const override;

    bool asBoolean() const override;

    std::string asString() const override;

    std::vector<std::shared_ptr<Value>> asArray() const override;

    void swap(Value &other) override;

    ValueType getElemsType() const;

    size_t getSize() const;
};

#endif // ARRAY_VALUE_H
