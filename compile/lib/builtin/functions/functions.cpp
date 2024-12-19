#include "functions.h"
#include "../../double_value/double_value.h"
#include "../../int_value/int_value.h"
#include <cmath>
#include <iostream>

class SinFunction : public Function {
public:
    std::shared_ptr<Value> execute(const std::vector<std::shared_ptr<Value>>& args) override {
        if (args.size() != 1) throw std::runtime_error("One argument expected for sin");
        return std::make_shared<DoubleValue>(std::sin(args[0]->asDouble()));
    }
};

class CosFunction : public Function {
public:
    std::shared_ptr<Value> execute(const std::vector<std::shared_ptr<Value>>& args) override {
        if (args.size() != 1) throw std::runtime_error("One argument expected for cos");
        return std::make_shared<DoubleValue>(std::cos(args[0]->asDouble()));
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
        return std::make_shared<IntValue>(factorial(args[0]->asInt()));
    }
};

class ToastFunction : public Function {
public:
    std::shared_ptr<Value> execute(const std::vector<std::shared_ptr<Value>>& args) override {
        for (const auto& arg : args) {
            std::cout << arg->asString();
        }
        return std::make_shared<IntValue>(0);
    }
};

class ToastnFunction : public Function {
public:
    std::shared_ptr<Value> execute(const std::vector<std::shared_ptr<Value>>& args) override {
        for (const auto& arg : args) {
            std::cout << arg->asString() << std::endl;
        }
        return std::make_shared<IntValue>(0);
    }
};

std::unordered_map<std::string, std::shared_ptr<Function>> Functions::functions;

void Functions::initialize() {
    functions["sin"] = std::make_shared<SinFunction>();
    functions["cos"] = std::make_shared<CosFunction>();
    functions["toast"] = std::make_shared<ToastFunction>();
    functions["toastn"] = std::make_shared<ToastnFunction>();
    functions["fucktorial"] = std::make_shared<FactorialFunction>();
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
