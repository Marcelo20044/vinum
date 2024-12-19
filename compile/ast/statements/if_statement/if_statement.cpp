#include "if_statement.h"

IfStatement::IfStatement(std::shared_ptr<Expression> expression,
                         std::shared_ptr<Statement> ifStatement,
                         std::shared_ptr<Statement> elseStatement)
    : expression(std::move(expression)), ifStatement(std::move(ifStatement)), elseStatement(std::move(elseStatement)) {
}

void IfStatement::execute() {
    if (const bool result = expression->eval()->asBoolean(); result != false) {
        ifStatement->execute();
    } else if (elseStatement != nullptr) {
        elseStatement->execute();
    }
}
