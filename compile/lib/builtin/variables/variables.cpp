#include "variables.h"
#include "../../string_value/string_value.h"
#include "../../double_value/double_value.h"
#include <stdexcept>

std::stack<std::unordered_map<std::string, std::shared_ptr<Value>>> Variables::stack;
std::unordered_map<std::string, std::shared_ptr<Value>> Variables::variables;

void Variables::initializeConstants() {
    variables["PI"] = std::make_shared<DoubleValue>(M_PI);
    variables["E"] = std::make_shared<DoubleValue>(M_E);
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

std::shared_ptr<Value> Variables::get(const std::string& key) {
    if (!isExists(key)) {
        throw std::runtime_error("Undefined variable: " + key);
    }
    return variables[key];
}

void Variables::set(const std::string& key, std::shared_ptr<Value> value) {
    variables[key] = std::move(value);
}
