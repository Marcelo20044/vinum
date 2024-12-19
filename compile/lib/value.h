#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <unordered_map>

enum class ValueType {
    SHORT,
    INT,
    LONG,
    DOUBLE,
    STRING,
    BOOLEAN,
    VOID,
};

class Value {
    ValueType type;
    static const std::unordered_map<std::string, ValueType> types;

public:
    explicit Value(const ValueType type) : type(type) {
    }

    virtual ~Value() = default;

    ValueType getType() const { return type; }

    virtual short asShort() const = 0;

    virtual int asInt() const = 0;

    virtual long asLong() const = 0;

    virtual double asDouble() const = 0;

    virtual bool asBoolean() const = 0;

    virtual std::string asString() const = 0;

    static bool typeExists(const std::string &type);
    static ValueType getType(const std::string &type);
    static std::shared_ptr<Value> asType(std::shared_ptr<Value> value, ValueType type);
};

#endif // VALUE_H
