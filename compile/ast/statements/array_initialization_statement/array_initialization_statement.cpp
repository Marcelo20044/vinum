#include "array_initialization_statement.h"

#include "../../../lib/array_value/array_value.h"


ArrayInitializationStatement::ArrayInitializationStatement(ValueType elems_type, std::string arr_name,
                                                           const std::shared_ptr<Expression> &size,
                                                           const std::vector<std::shared_ptr<Expression> > &
                                                           elements) : elemsType(elems_type),
                                                                       arrName(std::move(arr_name)),
                                                                       size(size),
                                                                       elements(elements) {
}

void ArrayInitializationStatement::execute() {
    if (Variables::isExists(arrName)) {
        throw std::runtime_error("Variable '" + arrName + "' already exists");
    }

    size_t arrSize = size->eval()->asInt();

    if (elements.empty()) {
        Variables::set(arrName, std::make_shared<ArrayValue>(ArrayValue(arrSize, elemsType)));
    } else {
        if (elements.size() != arrSize) {
            throw std::runtime_error("Array size mismatch");
        }

        std::vector<std::shared_ptr<Value> > elems;
        elems.reserve(elements.size());
        for (const auto &value: elements) {
            elems.push_back(Value::asType(value->eval(), elemsType));
        }

        Variables::set(arrName, std::make_shared<ArrayValue>(ArrayValue(elemsType, elems)));
    }
}
