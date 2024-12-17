#include "functional_expression.h"
#include <stdexcept>
#include <utility>
#include "../../../lib/builtin/functions/functions.h"
#include "../../../lib/user_function/user_function.h"

FunctionalExpression::FunctionalExpression(std::string name) : name(std::move(name)) {
}

FunctionalExpression::FunctionalExpression(std::string name,
                                           const std::vector<std::shared_ptr<Expression> > &args)
    : name(std::move(name)), arguments(args) {
}

void FunctionalExpression::addArgument(std::shared_ptr<Expression> arg) {
    arguments.push_back(std::move(arg));
}

std::shared_ptr<Value> FunctionalExpression::eval() {
    size_t size = arguments.size();
    std::vector<std::shared_ptr<Value> > values(size);

    for (size_t i = 0; i < size; ++i) {
        values[i] = arguments[i]->eval();
    }

    std::shared_ptr<Function> function = Functions::get(name);

    if (auto userFunction = std::dynamic_pointer_cast<UserFunction>(function)) {
        if (size != userFunction->getArgsCount()) {
            throw std::runtime_error("Argument count mismatch");
        }

        Variables::push();
        for (size_t i = 0; i < size; ++i) {
            Variables::set(userFunction->getArgsName(i), values[i]);
        }
        std::shared_ptr<Value> result = userFunction->execute(values);
        Variables::pop();
        return result;
    }

    return function->execute(values);
}
