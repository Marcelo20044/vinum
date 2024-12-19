#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "../lexer/token.h"
#include "../ast/ast.h"
#include "../ast/expressions/functional_expression/functional_expression.h"

class Parser {
    static const std::shared_ptr<Token> EOF_TOKEN;
    const std::vector<Token> &tokens;
    size_t pos;
    size_t size;

    std::shared_ptr<Token> consume(TokenType type);

    bool match(TokenType type);

    std::shared_ptr<Token> get(int relativePosition);

    std::shared_ptr<Statement> block();

    std::shared_ptr<Statement> statementOrBlock();

    std::shared_ptr<Statement> statement();

    std::shared_ptr<Statement> assignmentStatement();

    std::shared_ptr<Statement> initializationStatement();

    std::shared_ptr<Statement> ifElse();

    std::shared_ptr<Statement> forStatement();

    std::shared_ptr<Statement> functionDefine();

    std::shared_ptr<FunctionalExpression> function();

    std::shared_ptr<Expression> expression();

    std::shared_ptr<Expression> logicalOr();

    std::shared_ptr<Expression> logicalAnd();

    std::shared_ptr<Expression> equality();

    std::shared_ptr<Expression> conditional();

    std::shared_ptr<Expression> additive();

    std::shared_ptr<Expression> multiplicative();

    std::shared_ptr<Expression> unary();

    std::shared_ptr<Expression> primary();

public:
    explicit Parser(const std::vector<Token> &tokens);

    std::shared_ptr<Statement> parse();
};

#endif // PARSER_H
