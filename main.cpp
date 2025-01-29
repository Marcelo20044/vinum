#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Verifier.h"
#include "runtime/jit/jit_compiler.h"
#include "runtime/vm/vm.h"

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include "compile/lexer/lexer.h"
#include "compile/lib/builtin/functions/functions.h"
#include "compile/parser/parser.h"
#include "compile/generator/generator.h"
#include <iostream>
#include <llvm/IR/Module.h>

void vm_exec(std::string IRFile)
{
    std::string const MAIN_FUNC_NAME = "main";

    VM vm;

    vm.initGC();

    if (!vm.loadIR(IRFile))
    {
        std::cerr << "Failed to load IR from file: " << IRFile << "\n";
    }
    vm.execute(MAIN_FUNC_NAME);
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
    } catch (const std::exception& ex) {
        std::cerr << "Lexer error: " << ex.what() << std::endl;
        return 1;
    }

    Functions::initialize();

    std::shared_ptr<BlockStatement> block;

    try {
        Parser parser(tokens);
        block = parser.parse();
    } catch (const std::exception& ex) {
        std::cerr << "Parser error: " << ex.what() << std::endl;
        return 1;
    }

    llvm::LLVMContext context;
    llvm::IRBuilder<> builder(context);
    llvm::Module module("main_module", context);
    Generator generator;

    try {
        generator.generateIR(block);
    } catch (const std::exception& ex) {
        std::cerr << "Generator error: " << ex.what() << std::endl;
        return 1;
    }
    llvm::sys::DynamicLibrary::LoadLibraryPermanently("libgc.so");

    vm_exec("../cmake-build-debug/output.ll");
}

