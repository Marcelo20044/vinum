#include "block_statement.h"

void BlockStatement::add(const std::shared_ptr<Statement> &statement) {
    statements.push_back(statement);
}

void BlockStatement::execute() {
    for (const auto &statement: statements) {
        statement->execute();
    }
}

std::vector<std::shared_ptr<Statement>> BlockStatement::getStatements() {
    return this->statements;
}
