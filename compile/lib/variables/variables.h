#ifndef VARIABLES_H
#define VARIABLES_H

#include <string>
#include <unordered_map>
#include <stack>
#include <memory>
#include "../value.h"
#include "../number_value/number_value.h"

class Variables {
public:
    static void push();
    static void pop();

    static bool isExists(const std::string& key);
    static std::shared_ptr<Value> get(const std::string& key);
    static void set(const std::string& key, std::shared_ptr<Value> value);

private:
    static std::stack<std::unordered_map<std::string, std::shared_ptr<Value>>> stack;
    static std::unordered_map<std::string, std::shared_ptr<Value>> variables;

    static void initializeConstants();
};

#endif // VARIABLES_H
