#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType {
    NUMBER,
    WORD,
    TEXT,
    // Keywords
    TOAST, // print
    IF,
    ELSE,
    DRINK, // for
    STOP, // break
    NEXT, // continue
    FUN, // func
    SOBER, // return
    // Operators
    PLUS, MINUS, STAR, SLASH, EQ, EQEQ, EXCL, EXCLEQ, LT, LTEQ, GT, GTEQ,
    BAR, BARBAR, AMP, AMPAMP, LPAREN, RPAREN, LBRACKET, RBRACKET, LBRACE, RBRACE, COMMA,
    EOF_TOKEN
};

struct Token {
    TokenType type;
    std::string text;
};

#endif // TOKEN_H
