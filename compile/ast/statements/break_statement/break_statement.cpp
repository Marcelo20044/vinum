#include "break_statement.h"

BreakStatement::BreakStatement()
    : std::runtime_error("Break statement") {}

void BreakStatement::execute() {
    throw *this; // Выбрасываем сам объект
}
