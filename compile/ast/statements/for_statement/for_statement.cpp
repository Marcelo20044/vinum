#include "for_statement.h"
#include "../break_statement/break_statement.h"
#include "../continue_statement/continue_statement.h"

ForStatement::ForStatement(std::shared_ptr<Statement> initialization,
                           std::shared_ptr<Expression> termination,
                           std::shared_ptr<Statement> increment,
                           std::shared_ptr<Statement> block)
    : initialization(std::move(initialization)),
      termination(std::move(termination)),
      increment(std::move(increment)),
      statement(std::move(block)) {
}

void ForStatement::execute() {
    for (initialization->execute(); termination->eval()->asNumber() != 0; increment->execute()) {
        try {
            statement->execute();
        } catch (const BreakStatement &) {
            break;
        } catch (const ContinueStatement &) {
            // continue
        }
    }
}
