#ifndef LEXER_H
#define LEXER_H



#include <string>
#include <unordered_map>
#include <vector>
#include "token.h"

class Lexer {
    static const std::unordered_map<std::string, TokenType> WORDS;
    static const std::unordered_map<std::string, TokenType> OPERATORS;

    std::string input;
    size_t length;
    std::vector<Token> tokens;
    size_t pos;

    char next();
    char peek(int relativePosition) const;
    void addToken(TokenType type, const std::string &text = "");
    void tokenizeNumber();
    void tokenizeHexNumber();
    void tokenizeOperator();
    void tokenizeWord();
    void tokenizeText();
    void tokenizeComment();
    void tokenizeMultilineComment();
    static bool isHexNumber(char current);

public:
    explicit Lexer(const std::string &input);
    std::vector<Token> tokenize();
};



#endif //LEXER_H
