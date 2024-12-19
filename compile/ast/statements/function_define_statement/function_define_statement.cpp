#include "function_define_statement.h"
#include "../../../lib/function.h"
#include "../../../lib/builtin/functions/functions.h"
#include "../../../lib/user_function/user_function.h"

FunctionDefineStatement::FunctionDefineStatement(std::string name, ValueType returnType,
                                                 const std::vector<std::shared_ptr<Argument> > &args,
                                                 std::shared_ptr<Statement> body)
    : name(std::move(name)), returnType(returnType), args(args), body(std::move(body)) {
}

void FunctionDefineStatement::execute() {
    const auto userFunction = std::make_shared<UserFunction>(returnType, args, body);
    Functions::set(name, userFunction);
}
