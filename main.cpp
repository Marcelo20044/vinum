#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "compile/lexer/lexer.h"
#include "compile/lib/builtin/functions/functions.h"
#include "compile/parser/parser.h"

std::string tokenTypeToString(const TokenType type) {
    switch (type) {
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::WORD: return "WORD";
        case TokenType::TEXT: return "TEXT";
        case TokenType::TOAST: return "TOAST"; // print
        case TokenType::IF: return "IF";
        case TokenType::ELSE: return "ELSE";
        case TokenType::DRINK: return "DRINK"; // for
        case TokenType::STOP: return "STOP"; // break
        case TokenType::NEXT: return "NEXT"; // continue
        case TokenType::FUN: return "FUN"; // func
        case TokenType::SOBER: return "SOBER"; // return
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::STAR: return "STAR";
        case TokenType::SLASH: return "SLASH";
        case TokenType::EQ: return "EQ";
        case TokenType::EQEQ: return "EQEQ";
        case TokenType::EXCL: return "EXCL";
        case TokenType::EXCLEQ: return "EXCLEQ";
        case TokenType::LT: return "LT";
        case TokenType::LTEQ: return "LTEQ";
        case TokenType::GT: return "GT";
        case TokenType::GTEQ: return "GTEQ";
        case TokenType::BAR: return "BAR";
        case TokenType::BARBAR: return "BARBAR";
        case TokenType::AMP: return "AMP";
        case TokenType::AMPAMP: return "AMPAMP";
        case TokenType::LPAREN: return "LPAREN";
        case TokenType::RPAREN: return "RPAREN";
        case TokenType::LBRACKET: return "LBRACKET";
        case TokenType::RBRACKET: return "RBRACKET";
        case TokenType::LBRACE: return "LBRACE";
        case TokenType::RBRACE: return "RBRACE";
        case TokenType::COMMA: return "COMMA";
        case TokenType::EOF_TOKEN: return "EOF_TOKEN";
        default: return "UNKNOWN";
    }
}

void printTokens(const std::vector<Token>& tokens) {
    for (const auto& token : tokens) {
        std::cout << "Token: { Type: " << tokenTypeToString(token.type) << ", Text: \"" << token.text << "\" }" << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <file.vnm>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    if (filename.substr(filename.find_last_of('.')) != ".vnm") {
        std::cerr << "Error: File must have a .vinum extension." << std::endl;
        return 1;
    }

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return 1;
    }

    std::string line;
    std::string input;
    while (std::getline(file, line)) {
        input += line + "\n";
    }
    file.close();

    std::vector<Token> tokens;

    try {
        Lexer lexer(input);
        tokens = lexer.tokenize();
        // printTokens(tokens);
    } catch (const std::exception& ex) {
        std::cerr << "Lexer error: " << ex.what() << std::endl;
        return 1;
    }

    Functions::initialize();

    try {
        Parser parser(tokens);
        parser.parse()->execute();
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Parser error: " << ex.what() << std::endl;
        return 1;
    }
}
