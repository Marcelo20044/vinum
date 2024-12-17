#include "user_function.h"
#include "../../ast/statements/return_statement/return_statement.h"
#include "../number_value/number_value.h"
#include <stdexcept>

UserFunction::UserFunction(const std::vector<std::string>& argNames, std::shared_ptr<Statement> body)
    : argNames(argNames), body(std::move(body)) {}

int UserFunction::getArgsCount() const {
    return argNames.size();
}

std::string UserFunction::getArgsName(int index) const {
    if (index < 0 || index >= getArgsCount()) {
        return "";
    }
    return argNames[index];
}

std::shared_ptr<Value> UserFunction::execute(const std::vector<std::shared_ptr<Value>>& args) {
    if (args.size() != argNames.size()) {
        throw std::runtime_error("Argument count mismatch");
    }

    Variables::push();
    for (size_t i = 0; i < args.size(); ++i) {
        Variables::set(argNames[i], args[i]);
    }

    try {
        body->execute();
        Variables::pop();
        return std::make_shared<NumberValue>(0);
    } catch (const ReturnStatement& rt) {
        Variables::pop();
        return rt.getResult();
    } catch (...) {
        Variables::pop();
        throw;
    }
}
