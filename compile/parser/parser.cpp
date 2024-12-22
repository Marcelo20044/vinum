#include "parser.h"
#include "../ast/expressions/value_expression/value_expression.h"
#include "../ast/expressions/unary_expression/unary_expression.h"
#include "../ast/expressions/binary_expression/binary_expression.h"
#include "../ast/expressions/variable_expression/variable_expression.h"
#include "../ast/expressions/conditional_expression/conditional_expression.h"
#include "../ast/statements/if_statement/if_statement.h"
#include "../ast/statements/for_statement/for_statement.h"
#include "../ast/statements/block_statement/block_statement.h"
#include "../ast/statements/break_statement/break_statement.h"
#include "../ast/statements/print_statement/print_statement.h"
#include "../ast/statements/return_statement/return_statement.h"
#include "../ast/statements/function_statement/function_statement.h"
#include "../ast/statements/continue_statement/continue_statement.h"
#include "../ast/statements/assigment_statement/assigment_statement.h"
#include "../ast/statements/function_define_statement/function_define_statement.h"
#include <stdexcept>

#include "../ast/statements/initialization_statement/initialization_statement.h"
#include "../lib/user_function/user_function.h"

const std::shared_ptr<Token> Parser::EOF_TOKEN = std::make_shared<Token>(Token(TokenType::EOF_TOKEN, ""));

Parser::Parser(const std::vector<Token>& tokens)
    : tokens(tokens), pos(0), size(tokens.size()) {}

std::shared_ptr<BlockStatement> Parser::parse() {
    auto result = std::make_shared<BlockStatement>();
    while (!match(TokenType::EOF_TOKEN)) {
        result->add(statement());
    }
    return result;
}

std::shared_ptr<Statement> Parser::block() {
    auto block = std::make_shared<BlockStatement>();
    consume(TokenType::LBRACE);
    while (!match(TokenType::RBRACE)) {
        block->add(statement());
    }
    return block;
}

std::shared_ptr<Statement> Parser::statementOrBlock() {
    if (get(0)->type == TokenType::LBRACE) return block();
    return statement();
}

std::shared_ptr<Statement> Parser::statement() {
    if (match(TokenType::TOAST)) {
        return std::make_shared<PrintStatement>(expression());
    }
    if (match(TokenType::BEEF)) {
        return ifElse();
    }
    if (match(TokenType::STOP)) {
        return std::make_shared<BreakStatement>();
    }
    if (match(TokenType::NEXT)) {
        return std::make_shared<ContinueStatement>();
    }
    if (match(TokenType::REFILL)) {
        return std::make_shared<ReturnStatement>(expression());
    }
    if (match(TokenType::POUR)) {
        return forStatement();
    }
    if (match(TokenType::FUN)) {
        return functionDefine();
    }
    if (get(0)->type == TokenType::WORD && get(1)->type == TokenType::LPAREN) {
        return std::make_shared<FunctionStatement>(function());
    }
    if (get(0)->type == TokenType::VARTYPE && get(1)->type == TokenType::WORD) {
        return initializationStatement();
    }
    return assignmentStatement();
}

std::shared_ptr<Statement> Parser::assignmentStatement() {
    std::shared_ptr<Token> current = get(0);
    if (match(TokenType::WORD) && get(0)->type == TokenType::EQ) {
        std::string variable = current->text;
        consume(TokenType::EQ);
        return std::make_shared<AssignmentStatement>(variable, expression());
    }
    throw std::runtime_error("Unknown statement: " + current->text);
}

std::shared_ptr<Statement> Parser::initializationStatement() {
    ValueType type = Value::getType(consume(TokenType::VARTYPE)->text);
    std::string name = consume(TokenType::WORD)->text;
    std::shared_ptr<Token> current = get(0);
    consume(TokenType::EQ);
    return std::make_shared<InitializationStatement>(type, name, expression());
}

std::shared_ptr<Statement> Parser::ifElse() {
    auto condition = expression();
    auto ifStatement = statementOrBlock();
    std::shared_ptr<Statement> elseStatement = nullptr;
    if (match(TokenType::BELLS)) {
        elseStatement = statementOrBlock();
    }
    return std::make_shared<IfStatement>(condition, ifStatement, elseStatement);
}

std::shared_ptr<Statement> Parser::forStatement() {
    auto initialization = initializationStatement();
    consume(TokenType::SEMICOLON);
    auto termination = expression();
    consume(TokenType::SEMICOLON);
    auto increment = assignmentStatement();
    auto statement = statementOrBlock();
    return std::make_shared<ForStatement>(initialization, termination, increment, statement);
}

std::shared_ptr<Statement> Parser::functionDefine() {
    ValueType returnType;
    try {
        returnType = Value::getType(consume(TokenType::VARTYPE)->text);
    } catch (const std::runtime_error&) {
        returnType = ValueType::VOID;
    }

    std::string name = consume(TokenType::WORD)->text;
    consume(TokenType::LPAREN);
    std::vector<std::string> argNames;

    std::vector<std::shared_ptr<Argument>> args;
    while (!match(TokenType::RPAREN)) {
        ValueType argType = Value::getType(consume(TokenType::VARTYPE)->text);
        std::string argName = consume(TokenType::WORD)->text;
        args.push_back(std::make_shared<Argument>(argType, argName));
        match(TokenType::COMMA);
    }

    auto body = statementOrBlock();
    return std::make_shared<FunctionDefineStatement>(name, returnType, args, body);
}

std::shared_ptr<FunctionalExpression> Parser::function() {
    std::string name = consume(TokenType::WORD)->text;
    consume(TokenType::LPAREN);
    auto function = std::make_shared<FunctionalExpression>(name);
    while (!match(TokenType::RPAREN)) {
        function->addArgument(expression());
        match(TokenType::COMMA);
    }
    return function;
}

std::shared_ptr<Expression> Parser::expression() {
    return logicalOr();
}

std::shared_ptr<Expression> Parser::logicalOr() {
    auto result = logicalAnd();
    while (match(TokenType::BARBAR)) {
        result = std::make_shared<ConditionalExpression>(ConditionalExpression::Operator::OR, result, logicalAnd());
    }
    return result;
}

std::shared_ptr<Expression> Parser::logicalAnd() {
    auto result = equality();
    while (match(TokenType::AMPAMP)) {
        result = std::make_shared<ConditionalExpression>(ConditionalExpression::Operator::AND, result, equality());
    }
    return result;
}

std::shared_ptr<Expression> Parser::equality() {
    auto result = conditional();
    if (match(TokenType::EQEQ)) {
        return std::make_shared<ConditionalExpression>(ConditionalExpression::Operator::EQUALS, result, conditional());
    }
    if (match(TokenType::EXCLEQ)) {
        return std::make_shared<ConditionalExpression>(ConditionalExpression::Operator::NOT_EQUALS, result, conditional());
    }
    return result;
}

std::shared_ptr<Expression> Parser::conditional() {
    auto result = additive();
    while (true) {
        if (match(TokenType::LT)) {
            result = std::make_shared<ConditionalExpression>(ConditionalExpression::Operator::LT, result, additive());
            continue;
        }
        if (match(TokenType::LTEQ)) {
            result = std::make_shared<ConditionalExpression>(ConditionalExpression::Operator::LTEQ, result, additive());
            continue;
        }
        if (match(TokenType::GT)) {
            result = std::make_shared<ConditionalExpression>(ConditionalExpression::Operator::GT, result, additive());
            continue;
        }
        if (match(TokenType::GTEQ)) {
            result = std::make_shared<ConditionalExpression>(ConditionalExpression::Operator::GTEQ, result, additive());
            continue;
        }
        break;
    }
    return result;
}

std::shared_ptr<Expression> Parser::additive() {
    auto result = multiplicative();
    while (true) {
        if (match(TokenType::PLUS)) {
            result = std::make_shared<BinaryExpression>('+', result, multiplicative());
            continue;
        }
        if (match(TokenType::MINUS)) {
            result = std::make_shared<BinaryExpression>('-', result, multiplicative());
            continue;
        }
        break;
    }
    return result;
}

std::shared_ptr<Expression> Parser::multiplicative() {
    auto result = unary();
    while (true) {
        if (match(TokenType::STAR)) {
            result = std::make_shared<BinaryExpression>('*', result, unary());
            continue;
        }
        if (match(TokenType::SLASH)) {
            result = std::make_shared<BinaryExpression>('/', result, unary());
            continue;
        }
        break;
    }
    return result;
}

std::shared_ptr<Expression> Parser::unary() {
    if (match(TokenType::MINUS)) {
        return std::make_shared<UnaryExpression>('-', primary());
    }
    if (match(TokenType::PLUS)) {
        return primary();
    }
    return primary();
}

std::shared_ptr<Expression> Parser::primary() {
    std::shared_ptr<Token> current = get(0);
    if (match(TokenType::NUMBER)) {
        const std::string& text = current->text;

        if (text.find('.') == std::string::npos && text.find('e') == std::string::npos && text.find('E') == std::string::npos) {
            try {
                int intValue = std::stoi(text);
                return std::make_shared<ValueExpression>(intValue);
            } catch (const std::out_of_range&) {

            }
        }

        try {
            double doubleValue = std::stod(text);
            return std::make_shared<ValueExpression>(doubleValue);
        } catch (const std::invalid_argument&) {
            throw std::runtime_error("Invalid number format: " + text);
        }
    }

    if (get(0)->type == TokenType::WORD && get(1)->type == TokenType::LPAREN) {
        return function();
    }
    if (match(TokenType::WORD)) {
        return std::make_shared<VariableExpression>(current->text);
    }
    if (match(TokenType::TEXT)) {
        return std::make_shared<ValueExpression>(current->text);
    }
    if (match(TokenType::LPAREN)) {
        auto result = expression();
        consume(TokenType::RPAREN);
        return result;
    }
    throw std::runtime_error("Unknown expression: " + current->text);
}

std::shared_ptr<Token> Parser::consume(TokenType type) {
    std::shared_ptr<Token> current = get(0);
    if (type != current->type) {
        throw std::runtime_error("Token " + current->text + " doesn't match expected type");
    }
    pos++;
    return current;
}

bool Parser::match(TokenType type) {
    if (get(0)->type != type) return false;
    pos++;
    return true;
}

std::shared_ptr<Token> Parser::get(int relativePosition) {
    size_t position = pos + relativePosition;
    if (position >= size) return EOF_TOKEN;
    return std::make_shared<Token>(tokens[position]);
}
