#include "function_define_statement.h"
#include "../../../lib/function.h"
#include "../../../lib/builtin/functions/functions.h"
#include "../../../lib/user_function/user_function.h"

FunctionDefineStatement::FunctionDefineStatement(std::string name, const std::vector<std::string> &argNames,
                                                 std::shared_ptr<Statement> body)
    : name(std::move(name)), argNames(argNames), body(std::move(body)) {
}

void FunctionDefineStatement::execute() {
    const auto userFunction = std::make_shared<UserFunction>(argNames, body);
    Functions::set(name, userFunction);
}
