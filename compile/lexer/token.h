#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType {
    NUMBER,
    INT,
    LONG,
    DOUBLE,
    WORD,
    TEXT,
    // Keywords
    VARTYPE,
    TOAST, // print
    BEEF, // if
    BELLS, // else
    POUR, // for
    STOP, // break
    NEXT, // continue
    FUN, // func
    REFILL, // return
    // Operators
    PLUS, MINUS, STAR, SLASH, EQ, EQEQ, EXCL, EXCLEQ, LT, LTEQ, GT, GTEQ, REMDIV,
    BAR, BARBAR, AMP, AMPAMP, LPAREN, RPAREN, LBRACKET, RBRACKET, LBRACE, RBRACE, COMMA, SEMICOLON,
    EOF_TOKEN
};

struct Token {
    TokenType type;
    std::string text;

    Token(const TokenType type, std::string text) : type(type), text(std::move(text)) {
    }
};

#endif // TOKEN_H
