#include "variables.h"
#include "../string_value/string_value.h"
#include <cmath>
#include <stdexcept>

std::stack<std::unordered_map<std::string, std::shared_ptr<Value>>> Variables::stack;
std::unordered_map<std::string, std::shared_ptr<Value>> Variables::variables;

void Variables::initializeConstants() {
    variables["PI"] = std::make_shared<NumberValue>(M_PI);
    variables["ПИ"] = std::make_shared<NumberValue>(M_PI);
    variables["E"] = std::make_shared<NumberValue>(M_E);
    variables["GOLDEN_RATIO"] = std::make_shared<NumberValue>(1.618);
}

void Variables::push() {
    stack.push(variables);
}

void Variables::pop() {
    if (stack.empty()) {
        throw std::runtime_error("Variables stack underflow");
    }
    variables = stack.top();
    stack.pop();
}

bool Variables::isExists(const std::string& key) {
    return variables.find(key) != variables.end();
}

Value* Variables::get(const std::string& key) {
    if (!isExists(key)) {
        return const_cast<NumberValue*>(&NumberValue::ZERO);
    }
    return variables[key].get();
}

void Variables::set(const std::string& key, std::shared_ptr<Value> value) {
    variables[key] = value;
}
