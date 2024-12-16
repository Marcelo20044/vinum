#include "parser.h"
#include <stdexcept>
#include "../ast/expressions/unary_expression/unary_expression.h"
#include "../ast/expressions/binary_expression/binary_expression.h"
#include "../ast/expressions/value_expression/value_expression.h"
#include "../ast/expressions/variable_expression/variable_expression.h"
#include "../ast/expressions/conditional_expression/conditional_expression.h"
#include "../ast/statements/block_statement/block_statement.h"
#include "../ast/statements/assigment_statement/assigment_statement.h"
#include "../ast/statements/break_statement/break_statement.h"

const Token Parser::EOF_TOKEN = Token(TokenType::EOF_TOKEN, "");

Parser::Parser(const std::vector<Token>& tokens)
    : tokens(tokens), pos(0), size(tokens.size()) {}

Statement* Parser::parse() {
    auto* result = new BlockStatement();
    while (!match(TokenType::EOF_TOKEN)) {
        result->add(statement());
    }
    return result;
}

Statement* Parser::block() {
    auto* block = new BlockStatement();
    consume(TokenType::LBRACE);
    while (!match(TokenType::RBRACE)) {
        block->add(statement());
    }
    return block;
}

Statement* Parser::statementOrBlock() {
    if (get(0).type == TokenType::LBRACE) return block();
    return statement();
}

Statement* Parser::statement() {
    if (match(TokenType::TOAST)) {
        return new PrintStatement(expression());
    }
    if (match(TokenType::IF)) {
        return ifElse();
    }
    if (match(TokenType::STOP)) {
        return new BreakStatement();
    }
    if (match(TokenType::NEXT)) {
        return new ContinueStatement();
    }
    if (match(TokenType::SOBER)) {
        return new ReturnStatement(expression());
    }
    if (match(TokenType::DRINK)) {
        return forStatement();
    }
    if (match(TokenType::FUN)) {
        return functionDefine();
    }
    if (get(0).type == TokenType::WORD && get(1).type == TokenType::LPAREN) {
        return new FunctionStatement(function());
    }
    return assignmentStatement();
}

Statement* Parser::assignmentStatement() {
    Token current = get(0);
    if (match(TokenType::WORD) && get(0).type == TokenType::EQ) {
        std::string variable = current.text;
        consume(TokenType::EQ);
        return new AssignmentStatement(variable, expression());
    }
    throw std::runtime_error("Unknown statement");
}

Statement* Parser::ifElse() {
    Expression* condition = expression();
    Statement* ifStatement = statementOrBlock();
    Statement* elseStatement = nullptr;
    if (match(TokenType::ELSE)) {
        elseStatement = statementOrBlock();
    }
    return new IfStatement(condition, ifStatement, elseStatement);
}

Statement* Parser::forStatement() {
    Statement* initialization = assignmentStatement();
    consume(TokenType::COMMA);
    Expression* termination = expression();
    consume(TokenType::COMMA);
    Statement* increment = assignmentStatement();
    Statement* statement = statementOrBlock();
    return new ForStatement(initialization, termination, increment, statement);
}

FunctionDefineStatement* Parser::functionDefine() {
    std::string name = consume(TokenType::WORD).text;
    consume(TokenType::LPAREN);
    std::vector<std::string> argNames;
    while (!match(TokenType::RPAREN)) {
        argNames.push_back(consume(TokenType::WORD).text);
        match(TokenType::COMMA);
    }
    Statement* body = statementOrBlock();
    return new FunctionDefineStatement(name, argNames, body);
}

FunctionalExpression* Parser::function() {
    std::string name = consume(TokenType::WORD).text;
    consume(TokenType::LPAREN);
    auto* function = new FunctionalExpression(name);
    while (!match(TokenType::RPAREN)) {
        function->addArgument(expression());
        match(TokenType::COMMA);
    }
    return function;
}

Expression* Parser::expression() {
    return logicalOr();
}

Expression* Parser::logicalOr() {
    Expression* result = logicalAnd();
    while (match(TokenType::BARBAR)) {
        result = new ConditionalExpression(ConditionalExpression::Operator::OR, result, logicalAnd());
    }
    return result;
}

Expression* Parser::logicalAnd() {
    Expression* result = equality();
    while (match(TokenType::AMPAMP)) {
        result = new ConditionalExpression(ConditionalExpression::Operator::AND, result, equality());
    }
    return result;
}

Expression* Parser::equality() {
    Expression* result = conditional();
    if (match(TokenType::EQEQ)) {
        return new ConditionalExpression(ConditionalExpression::Operator::EQUALS, result, conditional());
    }
    if (match(TokenType::EXCLEQ)) {
        return new ConditionalExpression(ConditionalExpression::Operator::NOT_EQUALS, result, conditional());
    }
    return result;
}

Expression* Parser::conditional() {
    Expression* result = additive();
    while (true) {
        if (match(TokenType::LT)) {
            result = new ConditionalExpression(ConditionalExpression::Operator::LT, result, additive());
            continue;
        }
        if (match(TokenType::LTEQ)) {
            result = new ConditionalExpression(ConditionalExpression::Operator::LTEQ, result, additive());
            continue;
        }
        if (match(TokenType::GT)) {
            result = new ConditionalExpression(ConditionalExpression::Operator::GT, result, additive());
            continue;
        }
        if (match(TokenType::GTEQ)) {
            result = new ConditionalExpression(ConditionalExpression::Operator::GTEQ, result, additive());
            continue;
        }
        break;
    }
    return result;
}

Expression* Parser::additive() {
    Expression* result = multiplicative();
    while (true) {
        if (match(TokenType::PLUS)) {
            result = new BinaryExpression('+', result, multiplicative());
            continue;
        }
        if (match(TokenType::MINUS)) {
            result = new BinaryExpression('-', result, multiplicative());
            continue;
        }
        break;
    }
    return result;
}

Expression* Parser::multiplicative() {
    Expression* result = unary();
    while (true) {
        if (match(TokenType::STAR)) {
            result = new BinaryExpression('*', result, unary());
            continue;
        }
        if (match(TokenType::SLASH)) {
            result = new BinaryExpression('/', result, unary());
            continue;
        }
        break;
    }
    return result;
}

Expression* Parser::unary() {
    if (match(TokenType::MINUS)) {
        return new UnaryExpression('-', primary());
    }
    if (match(TokenType::PLUS)) {
        return primary();
    }
    return primary();
}

Expression* Parser::primary() {
    Token current = get(0);
    if (match(TokenType::NUMBER)) {
        return new ValueExpression(std::stod(current.text));
    }
    if (get(0).type == TokenType::WORD && get(1).type == TokenType::LPAREN) {
        return function();
    }
    if (match(TokenType::WORD)) {
        return new VariableExpression(current.text);
    }
    if (match(TokenType::TEXT)) {
        return new ValueExpression(current.text);
    }
    if (match(TokenType::LPAREN)) {
        Expression* result = expression();
        consume(TokenType::RPAREN);
        return result;
    }
    throw std::runtime_error("Unknown expression");
}

Token Parser::consume(TokenType type) {
    Token current = get(0);
    if (type != current.type) {
        throw std::runtime_error("Token " + current.text + " doesn't match " + tokenTypeToString(type));
    }
    pos++;
    return current;
}

bool Parser::match(TokenType type) {
    if (get(0).type != type) return false;
    pos++;
    return true;
}

Token Parser::get(int relativePosition) {
    size_t position = pos + relativePosition;
    if (position >= size) return EOF_TOKEN;
    return tokens[position];
}
