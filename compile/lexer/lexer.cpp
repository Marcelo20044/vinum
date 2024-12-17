#include "lexer.h"
#include <cctype>
#include <stdexcept>
#include <sstream>

const std::unordered_map<std::string, TokenType> Lexer::WORDS = {
    {"toast", TokenType::TOAST}, {"if", TokenType::IF}, {"else", TokenType::ELSE},
    {"drink", TokenType::DRINK}, {"stop", TokenType::STOP}, {"next", TokenType::NEXT},
    {"fun", TokenType::FUN}, {"sober", TokenType::SOBER}
};

const std::unordered_map<std::string, TokenType> Lexer::OPERATORS = {
    {"+", TokenType::PLUS}, {"-", TokenType::MINUS}, {"*", TokenType::STAR},
    {"/", TokenType::SLASH}, {"(", TokenType::LPAREN}, {")", TokenType::RPAREN},
    {"[", TokenType::LBRACKET}, {"]", TokenType::RBRACKET}, {"{", TokenType::LBRACE},
    {"}", TokenType::RBRACE}, {"=", TokenType::EQ}, {"<", TokenType::LT},
    {">", TokenType::GT}, {",", TokenType::COMMA}, {"!", TokenType::EXCL},
    {"&", TokenType::AMP}, {"|", TokenType::BAR}, {"==", TokenType::EQEQ},
    {"!=", TokenType::EXCLEQ}, {"<=", TokenType::LTEQ}, {">=", TokenType::GTEQ},
    {"&&", TokenType::AMPAMP}, {"||", TokenType::BARBAR}, {";", TokenType::SEMICOLON}
};

Lexer::Lexer(const std::string &input) : input(input), length(input.length()), pos(0) {
}

std::vector<Token> Lexer::tokenize() {
    while (pos < length) {
        if (const char current = peek(0); std::isdigit(current)) tokenizeNumber();
        else if (std::isalpha(current)) tokenizeWord();
        else if (current == '"') tokenizeText();
        else if (OPERATORS.contains(std::string(1, current))) {
            tokenizeOperator();
        } else {
            next(); // Skip whitespace or unrecognized characters
        }
    }
    return tokens;
}

char Lexer::peek(const int relativePosition) const {
    const size_t position = pos + relativePosition;
    return (position >= length) ? '\0' : input[position];
}

char Lexer::next() {
    pos++;
    return peek(0);
}

void Lexer::addToken(const TokenType type, const std::string &text) {
    tokens.push_back(Token{type, text});
}

void Lexer::tokenizeNumber() {
    std::string buffer;
    char current = peek(0);
    while (std::isdigit(current)) {
        if (current == '.') {
            if (buffer.find('.') != std::string::npos) throw std::runtime_error("Invalid float number");
        }

        buffer += current;
        current = next();
    }
    addToken(TokenType::NUMBER, buffer);
}

void Lexer::tokenizeOperator() {
    char current = peek(0);
    if (current == '/') {
        if (peek(1) == '/') {
            next();
            next();
            tokenizeComment();
            return;
        }
        if (peek(1) == '*') {
            next();
            next();
            tokenizeMultilineComment();
            return;
        }
    }

    std::string buffer;
    while (true) {
        if (std::string text = buffer; !OPERATORS.contains(text + current) && !text.empty()) {
            addToken(OPERATORS.at(text));
            return;
        }
        buffer += current;
        current = next();
    }
}

void Lexer::tokenizeWord() {
    std::string word;
    char current = peek(0);
    while (std::isalnum(current) || current == '_' || current == '$') {
        word += current;
        current = next();
    }

    if (WORDS.contains(word)) {
        addToken(WORDS.at(word));
    } else {
        addToken(TokenType::WORD, word);
    }
}

void Lexer::tokenizeText() {
    next(); // skip "
    std::string buffer;

    char current = peek(0);
    while (current != '\0') {
        if (current == '\\') {
            current = next();
            switch (current) {
                case '"': buffer += '"'; break;
                case 'n': buffer += '\n'; break;
                case 't': buffer += '\t'; break;
                default: buffer += '\\'; buffer += current; break;
            }
        } else if (current == '"') {
            break;
        } else {
            buffer += current;
        }
        current = next();
    }

    next(); // skip closing "
    addToken(TokenType::TEXT, buffer);
}

void Lexer::tokenizeComment() {
    char current = peek(0);
    while (current != '\r' && current != '\n' && current != '\0') {
        current = next();
    }
}

void Lexer::tokenizeMultilineComment() {
    char current = peek(0);
    while (true) {
        if (current == '\0') throw std::runtime_error("Missing close tag");
        if (current == '*' && peek(1) == '/') break;
        current = next();
    }
    next(); // *
    next(); // /
}
