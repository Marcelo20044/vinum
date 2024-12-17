#include "functions.h"
#include "../../number_value/number_value.h"
#include <cmath>
#include <iostream>

class SinFunction : public Function {
public:
    std::shared_ptr<Value> execute(const std::vector<std::shared_ptr<Value>>& args) override {
        if (args.size() != 1) throw std::runtime_error("One argument expected for sin");
        return std::make_shared<NumberValue>(std::sin(args[0]->asNumber()));
    }
};

class CosFunction : public Function {
public:
    std::shared_ptr<Value> execute(const std::vector<std::shared_ptr<Value>>& args) override {
        if (args.size() != 1) throw std::runtime_error("One argument expected for cos");
        return std::make_shared<NumberValue>(std::cos(args[0]->asNumber()));
    }
};

class FactorialFunction : public Function {
    static int factorial(const int x) {
        if (x == 0) return 1;
        return x * factorial(x - 1);
    }
public:
    std::shared_ptr<Value> execute(const std::vector<std::shared_ptr<Value>>& args) override {
        if (args.size() != 1) throw std::runtime_error("One argument expected for cos");
        return std::make_shared<NumberValue>(factorial(args[0]->asNumber()));
    }
};

std::unordered_map<std::string, std::shared_ptr<Function>> Functions::functions;

void Functions::initialize() {
    functions["sin"] = std::make_shared<SinFunction>();
    functions["cos"] = std::make_shared<CosFunction>();
    functions["fuck"] = std::make_shared<FactorialFunction>();
}

bool Functions::isExists(const std::string& key) {
    return functions.find(key) != functions.end();
}

std::shared_ptr<Function> Functions::get(const std::string& key) {
    if (!isExists(key)) {
        throw std::runtime_error("Unknown function: " + key);
    }
    return functions[key];
}

void Functions::set(const std::string& key, std::shared_ptr<Function> function) {
    functions[key] = std::move(function);
}
