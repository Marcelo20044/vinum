#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include "../lexer/token.h"
#include "../ast/ast.h" // Заголовок для Expression, Statement, Value и Function

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);
    Statement* parse();

private:
    const static Token EOF_TOKEN;

    const std::vector<Token>& tokens;
    size_t pos;
    size_t size;

    Statement* block();
    Statement* statementOrBlock();
    Statement* statement();
    Statement* assignmentStatement();
    Statement* ifElse();
    Statement* whileStatement();
    Statement* doWhileStatement();
    Statement* forStatement();
    Statement* functionDefine();

    Expression* function();
    Expression* expression();
    Expression* logicalOr();
    Expression* logicalAnd();
    Expression* equality();
    Expression* conditional();
    Expression* additive();
    Expression* multiplicative();
    Expression* unary();
    Expression* primary();

    Token consume(TokenType type);
    bool match(TokenType type);
    Token get(int relativePosition);
};

#endif // PARSER_H

