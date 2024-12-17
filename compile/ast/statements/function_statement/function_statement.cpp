#include "function_statement.h"

FunctionStatement::FunctionStatement(std::shared_ptr<FunctionalExpression> function)
    : function(std::move(function)) {
}

void FunctionStatement::execute() {
    function->eval();
}
