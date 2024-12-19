#include "user_function.h"
#include "../../ast/statements/return_statement/return_statement.h"
#include "../double_value/double_value.h"
#include <stdexcept>
#include "../int_value/int_value.h"

UserFunction::UserFunction(ValueType returnType, std::vector<std::shared_ptr<Argument>>& args, std::shared_ptr<Statement> body)
    : Function(), returnType(returnType), args(args), body(std::move(body)) {
}

int UserFunction::getArgsCount() const {
    return args.size();
}

std::string UserFunction::getArgsName(int index) const {
    if (index < 0 || index >= getArgsCount()) {
        return "";
    }
    return args[index]->name;
}

std::shared_ptr<Value> UserFunction::execute(const std::vector<std::shared_ptr<Value>>& argValues) {
    if (args.size() != argValues.size()) {
        throw std::runtime_error("Argument count mismatch");
    }

    Variables::push();
    for (size_t i = 0; i < args.size(); ++i) {
        std::shared_ptr<Value> value;
        try {
            value = Value::asType(argValues[i], args[i]->type);
        } catch (const std::runtime_error&) {
            throw std::runtime_error("Argument \"" + args[i]->name + "\" type mismatch");
        }
        Variables::set(args[i]->name, value);
    }

    try {
        body->execute();
        Variables::pop();
        return std::make_shared<IntValue>(0);
    } catch (const ReturnStatement& rt) {
        Variables::pop();
        if (returnType == ValueType::VOID) {
            return std::make_shared<IntValue>(0);
        }
        return Value::asType(rt.getResult(), returnType);
    } catch (...) {
        Variables::pop();
        throw;
    }
}