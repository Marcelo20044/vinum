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
    ARRAY,
    VOID,
};

class Value {

    static const std::unordered_map<std::string, ValueType> types;
    static const std::unordered_map<ValueType, std::string> reverseTypes;

public:
    ValueType type;

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

    virtual std::vector<std::shared_ptr<Value>> asArray() const = 0;

    virtual void swap(Value& other) = 0;

    static bool typeExists(const std::string &type);

    static ValueType getType(const std::string &type);

    static std::string getTypeString(ValueType type);

    static std::shared_ptr<Value> asType(std::shared_ptr<Value> value, ValueType type);
};

#endif // VALUE_H
