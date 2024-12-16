#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "compile/lexer/lexer.h"

void printTokens(const std::vector<Token>& tokens) {
    for (const auto& token : tokens) {
        std::cout << "Token: { Type: " << static_cast<int>(token.type) << ", Text: \"" << token.text << "\" }" << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <file.vinum>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    if (filename.substr(filename.find_last_of('.')) != ".vinum") {
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

    try {
        Lexer lexer(input);
        std::vector<Token> tokens = lexer.tokenize();
        printTokens(tokens);
    } catch (const std::exception& ex) {
        std::cerr << "Lexer error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}