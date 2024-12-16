#include "block_statement.h"

void BlockStatement::add(Statement* statement) {
    statements.push_back(statement);
}

void BlockStatement::execute() {
    for (auto* statement : statements) {
        statement->execute();
    }
}

BlockStatement::~BlockStatement() {
    for (auto* statement : statements) {
        delete statement;
    }
}