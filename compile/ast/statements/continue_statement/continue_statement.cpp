#include "continue_statement.h"

ContinueStatement::ContinueStatement()
    : std::runtime_error("Continue statement") {
}

void ContinueStatement::execute() {
    throw *this;
}
