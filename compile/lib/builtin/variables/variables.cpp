#include "variables.h"
#include "../../string_value/string_value.h"
#include "../../number_value/number_value.h"
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

std::shared_ptr<Value> Variables::get(const std::string& key) {
    if (!isExists(key)) {
        return std::make_shared<NumberValue>(0);
    }
    return variables[key];
}

void Variables::set(const std::string& key, std::shared_ptr<Value> value) {
    variables[key] = value;
}
